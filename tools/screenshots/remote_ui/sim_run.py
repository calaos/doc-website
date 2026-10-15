#!/usr/bin/env python3
"""Fake Calaos server + step driver for the calaos_remote_ui SDL simulator.

Runs inside the container. See README.md for the scenario format.
"""
import argparse
import base64
import hashlib
import hmac
import json
import os
import socket
import struct
import subprocess
import sys
import tempfile
import threading
import time
import zlib

WS_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
WS_PATH = "/api/v3/remote_ui/ws"
DEVICE_ID = "sim-panel"
AUTH_TOKEN = "sim-token"
DEVICE_SECRET = "sim-secret"

T0 = time.time()


def log(msg):
    print("[sim-run %6.2fs] %s" % (time.time() - T0, msg), flush=True)


def crc32(data):
    return zlib.crc32(data) & 0xFFFFFFFF


def local_ip():
    """Address of the container's own interface, falling back to loopback."""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("10.255.255.255", 1))
        return s.getsockname()[0]
    except OSError:
        return "127.0.0.1"
    finally:
        s.close()


def write_provisioning(cfg_dir, host, port):
    """Device config (skips discovery) + a completed provisioning record."""
    os.makedirs(cfg_dir, exist_ok=True)
    payload = json.dumps({
        "network_interface": "ethernet", "ip_mode": "dhcp",
        "static_ip": "", "static_mask": "", "static_gateway": "", "static_dns": "",
        "wifi_ssid": "", "wifi_password": "",
        "server_host": host, "server_port": port, "server_ssl": False,
    }, separators=(",", ":")).encode()
    header = b"CALO" + bytes([1, 0]) + struct.pack("<HI", len(payload), crc32(payload))
    with open(os.path.join(cfg_dir, "device_config.bin"), "wb") as f:
        f.write(header + payload)
    with open(os.path.join(cfg_dir, "prov.config"), "w") as f:
        json.dump({
            "provisioning_code": "SIM123", "salt": "00000000", "provisioned": True,
            "device_id": DEVICE_ID, "auth_token": AUTH_TOKEN, "device_secret": DEVICE_SECRET,
            "server_url": host, "mac_address": "",
        }, f)


class FakeServer:
    def __init__(self, scenario, out_dir, size):
        self.scenario = scenario
        self.out_dir = out_dir
        gw, gh = (4, 2) if size == (800, 480) else (3, 3)
        cfg = dict(scenario.get("config", {}))
        grid = scenario.get("grid")
        self.config = {"name": "Sim panel", "room": "Salon", "theme": "dark", "brightness": 80,
                       "grid_width": grid[0] if grid else gw,
                       "grid_height": grid[1] if grid else gh,
                       "pages": scenario.get("pages", [])}
        self.config.update(cfg)
        self.io = scenario.get("io_states", {})
        srv = scenario.get("server", {})
        self.reply = srv.get("reply", "success")
        self.echo = srv.get("echo", True)

        self.lock = threading.Lock()
        self.received = []          # (monotonic ts, raw text)
        self.conn = None
        self.conn_ready = threading.Event()
        self.auth_results = []
        self.send_lock = threading.Lock()
        self.set_state_log = open(os.path.join(out_dir, "set_state.jsonl"), "w")
        self.msg_log = open(os.path.join(out_dir, "server.log"), "w")

        self.sock = socket.socket()
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("0.0.0.0", 0))
        self.sock.listen(4)
        self.port = self.sock.getsockname()[1]
        threading.Thread(target=self._accept_loop, daemon=True).start()

    def _slog(self, line):
        self.msg_log.write("%.3f %s\n" % (time.time() - T0, line))
        self.msg_log.flush()

    def _accept_loop(self):
        while True:
            c, _ = self.sock.accept()
            threading.Thread(target=self._serve, args=(c,), daemon=True).start()

    @staticmethod
    def _read_exact(c, n):
        buf = b""
        while len(buf) < n:
            chunk = c.recv(n - len(buf))
            if not chunk:
                raise EOFError
            buf += chunk
        return buf

    def _handshake(self, c):
        data = b""
        while b"\r\n\r\n" not in data:
            chunk = c.recv(4096)
            if not chunk:
                self._slog("peer closed before the handshake completed")
                return False
            data += chunk
        head = data.split(b"\r\n\r\n", 1)[0].decode("latin1").split("\r\n")
        path = head[0].split(" ")[1]
        hdr = {}
        for line in head[1:]:
            k, _, v = line.partition(":")
            hdr[k.strip().lower()] = v.strip()
        token = hdr.get("authorization", "").replace("Bearer ", "")
        signed = "%s:%s:%s" % (token, hdr.get("x-auth-timestamp", ""), hdr.get("x-auth-nonce", ""))
        want = hmac.new(DEVICE_SECRET.encode(), signed.encode(), hashlib.sha256).hexdigest()
        ok = token == AUTH_TOKEN and hmac.compare_digest(want, hdr.get("x-auth-hmac", "").lower())
        self.auth_results.append(ok)
        self._slog("handshake path=%s hmac_ok=%s version=%s hw=%s" % (
            path, ok, hdr.get("x-device-version"), hdr.get("x-device-hardware-id")))
        if path != WS_PATH or "sec-websocket-key" not in hdr:
            c.sendall(b"HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n")
            return False
        accept = base64.b64encode(hashlib.sha1((hdr["sec-websocket-key"] + WS_GUID).encode()).digest()).decode()
        c.sendall(("HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\n"
                   "Connection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n" % accept).encode())
        return True

    def _send_frame(self, c, opcode, payload):
        n = len(payload)
        head = bytes([0x80 | opcode])
        if n < 126:
            head += bytes([n])
        elif n < 65536:
            head += bytes([126]) + struct.pack(">H", n)
        else:
            head += bytes([127]) + struct.pack(">Q", n)
        with self.send_lock:
            c.sendall(head + payload)

    def _read_frame(self, c):
        b0, b1 = self._read_exact(c, 2)
        n = b1 & 0x7F
        if n == 126:
            n = struct.unpack(">H", self._read_exact(c, 2))[0]
        elif n == 127:
            n = struct.unpack(">Q", self._read_exact(c, 8))[0]
        mask = self._read_exact(c, 4) if b1 & 0x80 else None
        data = self._read_exact(c, n) if n else b""
        if mask:
            data = bytes(b ^ mask[i % 4] for i, b in enumerate(data))
        return b0 & 0x0F, bool(b0 & 0x80), data

    def _serve(self, c):
        try:
            if not self._handshake(c):
                c.close()
                return
            self.conn = c
            self.push_config()
            self.push_io_states(self.io)
            self.conn_ready.set()
            text = b""
            while True:
                op, fin, data = self._read_frame(c)
                if op == 0x8:
                    self._send_frame(c, 0x8, data[:2])
                    break
                if op == 0x9:
                    self._send_frame(c, 0xA, data)
                elif op in (0x1, 0x0):
                    text += data
                    if fin:
                        self._on_text(text.decode("utf-8", "replace"))
                        text = b""
        except (EOFError, OSError):
            pass
        finally:
            if self.conn is c:
                self.conn = None
                self.conn_ready.clear()
            c.close()

    def send(self, msg):
        c = self.conn
        if c is None:
            raise RuntimeError("panel is not connected")
        raw = msg if isinstance(msg, str) else json.dumps(msg)
        self._slog("-> " + raw)
        self._send_frame(c, 0x1, raw.encode())

    def push_config(self):
        self.send({"msg": "remote_ui_config_update", "data": self.config})

    def push_io_states(self, states):
        self.send({"msg": "remote_ui_io_states", "data": states})

    def push_io_changed(self, item):
        self.send({"msg": "event", "data": {"type_str": "io_changed", "data": item}})

    def _on_text(self, raw):
        self._slog("<- " + raw)
        with self.lock:
            self.received.append((time.monotonic(), raw))
        try:
            msg = json.loads(raw)
        except ValueError:
            return
        if msg.get("msg") == "remote_ui_get_config":
            self.push_config()
        elif msg.get("msg") == "set_state":
            self.set_state_log.write(json.dumps({"t": round(time.time() - T0, 3), "message": msg}) + "\n")
            self.set_state_log.flush()
            self._answer_set_state(msg)

    def _answer_set_state(self, msg):
        data = msg.get("data", {})
        io_id, value = data.get("id"), data.get("value", "")
        if self.reply != "none" and msg.get("msg_id"):
            self.send({"msg": "set_state", "msg_id": msg["msg_id"],
                       "data": {"success": "false" if self.reply == "refuse" else "true"}})
        if self.reply == "refuse" or not self.echo or io_id not in self.io:
            return
        item = self.io[io_id]
        if not isinstance(item, dict):
            item = self.io[io_id] = {"state": str(item)}
        dev = item.get("device")
        if isinstance(value, str) and value.startswith("{") and dev:
            try:
                patch = json.loads(value)
            except ValueError:
                return
            dev.setdefault("state", {}).update(patch)
            if "mode" in patch:
                item["state"] = patch["mode"]
        elif dev:
            dev.setdefault("state", {})["mode"] = value
            item["state"] = value
        else:
            cur = str(item.get("state", "false"))
            item["state"] = ("false" if cur == "true" else "true") if value == "toggle" else value
        out = {"id": io_id, "state": item.get("state", "")}
        if dev:
            out["device"] = dev
        self.push_io_changed(out)

    def count(self):
        with self.lock:
            return len(self.received)

    def since(self, cursor):
        with self.lock:
            return [raw for _, raw in self.received[cursor:]]


class ShimClient:
    def __init__(self, path, timeout=30):
        deadline = time.time() + timeout
        while True:
            try:
                self.s = socket.socket(socket.AF_UNIX)
                self.s.connect(path)
                break
            except OSError:
                self.s.close()
                if time.time() > deadline:
                    raise RuntimeError("shim socket never came up (is SDL rendering?)")
                time.sleep(0.1)
        self.f = self.s.makefile("rw", buffering=1)

    def cmd(self, line):
        self.f.write(line + "\n")
        reply = self.f.readline().strip()
        if not reply.startswith("ok"):
            raise RuntimeError("shim: %s -> %s" % (line, reply or "no reply"))
        return reply.split()[1:]

    def frame_crc(self):
        return self.cmd("ping")[1]


class Failure(Exception):
    pass


def settle(shim, quiet_ms, cap_ms=1500):
    """Wait until the screen content stops changing (frames may keep being presented)."""
    last, since, start = shim.frame_crc(), time.time(), time.time()
    while time.time() - start < cap_ms / 1000:
        time.sleep(0.05)
        cur = shim.frame_crc()
        if cur != last:
            last, since = cur, time.time()
        elif time.time() - since >= quiet_ms / 1000:
            return True
    log("screen still changing after %d ms, capturing anyway" % cap_ms)
    return False


def run_steps(scn, server, shim, args, proc):
    size = tuple(int(v) for v in args.size.split("x"))
    results, failures = [], []
    cursor = 0
    steps = scn.get("steps", [])
    for idx, step in enumerate(steps):
        if proc.poll() is not None:
            raise RuntimeError("simulator exited with code %s" % proc.returncode)
        kind = next((k for k in ("wait", "tap", "send", "io_changed", "io_states", "shot",
                                 "expect_sent", "expect_not_sent", "server") if k in step), None)
        t0 = time.time()
        res = {"i": idx, "step": step}
        try:
            if kind == "wait":
                time.sleep(float(step["wait"]) / 1000)
            elif kind == "tap":
                x, y = step["tap"]
                cursor = server.count()
                shim.cmd("tap %d %d %d" % (x, y, step.get("hold", 80)))
            elif kind in ("send", "io_changed", "io_states"):
                cursor = server.count()
                if kind == "send":
                    server.send(step["send"])
                elif kind == "io_changed":
                    server.push_io_changed(step["io_changed"])
                else:
                    server.push_io_states(step["io_states"])
            elif kind == "server":
                for k in ("reply", "echo"):
                    if k in step["server"]:
                        setattr(server, k, step["server"][k])
            elif kind == "shot":
                name = step["shot"]
                settle(shim, step.get("settle", 300), step.get("settle_cap", 1500))
                path = os.path.join(args.out, name + ".png")
                w, h = (int(v) for v in shim.cmd("shot " + path))
                res["png"] = path
                if (w, h) != size:
                    raise Failure("frame is %dx%d, expected %dx%d" % (w, h, size[0], size[1]))
                log("shot %s (%dx%d)" % (path, w, h))
            elif kind == "expect_sent":
                needle = step["expect_sent"]
                deadline = time.time() + step.get("timeout", 3000) / 1000
                while True:
                    hit = [m for m in server.since(cursor) if needle in m]
                    if hit:
                        res["matched"] = hit[0]
                        break
                    if time.time() > deadline:
                        raise Failure("no message containing %r from the panel" % needle)
                    time.sleep(0.05)
            elif kind == "expect_not_sent":
                needle = step["expect_not_sent"]
                time.sleep(step.get("timeout", 1000) / 1000)
                hit = [m for m in server.since(cursor) if needle in m]
                if hit:
                    raise Failure("unexpected message containing %r: %s" % (needle, hit[0]))
            else:
                raise Failure("unknown step")
            res["ok"] = True
        except Failure as e:
            res.update(ok=False, error=str(e))
            failures.append(res)
            log("step %d FAILED: %s" % (idx, e))
        res["ms"] = int((time.time() - t0) * 1000)
        results.append(res)
    return results, failures


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--scenario", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--size", required=True)
    ap.add_argument("--binary", required=True)
    ap.add_argument("--shim", required=True)
    ap.add_argument("--timeout", type=int, default=120)
    ap.add_argument("--boot-wait", type=int, default=1500)
    args = ap.parse_args()

    with open(args.scenario) as f:
        scn = json.load(f)
    scn.update(scn.get("by_size", {}).get(args.size, {}))
    size = tuple(int(v) for v in args.size.split("x"))
    os.makedirs(args.out, exist_ok=True)

    run_dir = tempfile.mkdtemp(prefix="sim-run-")
    cfg_dir = os.path.join(run_dir, "cfg")
    server = FakeServer(scn, args.out, size)
    host = os.environ.get("SIM_SERVER_HOST") or local_ip()
    write_provisioning(cfg_dir, host, server.port)

    sock_path = os.path.join(run_dir, "shim.sock")
    env = dict(os.environ,
               HOME=run_dir, CALAOS_UI_CONFIG_PATH=cfg_dir,
               DISPLAY=":0", SDL_VIDEODRIVER="offscreen", SDL_AUDIODRIVER="dummy",
               CALAOS_DISPLAY_BACKEND="sdl", SIMSHOT_SOCK=sock_path, LD_PRELOAD=args.shim)
    app_log = open(os.path.join(args.out, "app.log"), "w")
    proc = subprocess.Popen([args.binary], env=env, cwd=run_dir, stdout=app_log, stderr=subprocess.STDOUT)
    log("simulator pid %d, fake server on %s:%d" % (proc.pid, host, server.port))

    code, results, failures, error = 0, [], [], None
    try:
        shim = ShimClient(sock_path)
        deadline = time.time() + args.timeout
        while not server.conn_ready.wait(0.2):
            if proc.poll() is not None:
                raise RuntimeError("simulator exited with code %s before connecting" % proc.returncode)
            if time.time() > deadline:
                raise RuntimeError("panel never connected to the fake server (see app.log)")
        log("panel connected (hmac ok: %s)" % server.auth_results)
        time.sleep(args.boot_wait / 1000)
        results, failures = run_steps(scn, server, shim, args, proc)
        code = 1 if failures else 0
    except Exception as e:
        error = str(e)
        log("ERROR: " + error)
        code = 2
    finally:
        if proc.poll() is None:
            proc.terminate()
            try:
                proc.wait(5)
            except subprocess.TimeoutExpired:
                proc.kill()
        app_log.close()

    with open(os.path.join(args.out, "result.json"), "w") as f:
        json.dump({"exit": code, "error": error, "hmac_ok": server.auth_results,
                   "steps": results}, f, indent=1)
    log("done: exit %d, %d step(s), %d failure(s)" % (code, len(results), len(failures)))
    sys.exit(code)


if __name__ == "__main__":
    main()
