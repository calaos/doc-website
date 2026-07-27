+++
title = 'Calaos Server'
date = 2024-03-06T20:17:51Z
weight = 10
summary = "The core of the system: hardware drivers, rules engine and JSON API."
+++

## Role

`calaos_server` is the only component that talks to the hardware. It loads the configuration, instantiates the inputs/outputs, runs the rules engine, records history and exposes an API consumed by every other component.

Repository: [calaos_base](https://github.com/calaos/calaos_base) — written in **C++**, released under **GPL v3**.

## Building

```sh
./autogen.sh
make
sudo make install
```

Main dependencies:

| Library | Use |
|---|---|
| libuv (> 1.10) | Event loop, networking, timers |
| jansson (> 2.5) | JSON serialisation |
| libcurl (> 7.20) | Outgoing HTTP requests |
| LuaJIT | Running user scripts |
| sigc++ (> 2.4.1) | Signals between objects |

And optionally, depending on the target hardware: **owfs** for 1-Wire, **OLA** for DMX, **knxd** for KNX, **ImageMagick** for processing camera pictures.

The repository also bundles several libraries in `src/lib`: `uvw` (a C++ wrapper over libuv), `llhttp`, `exprtk` for expression evaluation, `sqlite_modern_cpp`, `libquickmail` for sending emails, and `sunset.c` for computing sunrise and sunset times.

## Code layout

The server code lives in `src/bin/calaos_server`:

| Location | Contents |
|---|---|
| `Calaos.cpp` | Entry point of the business logic |
| `CalaosConfig.cpp` | Loading and writing the configuration |
| `IO/` | All input/output drivers |
| `IOBase.cpp`, `IOFactory.cpp` | Base class for IOs and its factory |
| `Rules/`, `Rule.cpp`, `ListeRule.cpp` | Rules engine |
| `Scenario/` | Scenarios |
| `LuaScript/` | LuaJIT integration |
| `JsonApi.cpp` | API implementation |
| `JsonApiHandlerWS.cpp`, `JsonApiHandlerHttp.cpp` | WebSocket and HTTP transports |
| `HttpServer.cpp`, `WebSocket.cpp`, `UDPServer.cpp` | Network servers |
| `EventManager.cpp` | Broadcasting events to clients |
| `HistLogger.cpp`, `DataLogger.cpp` | History and measurement recording |
| `NotifManager.cpp` | Email and push notifications |
| `IPCam/` | IP cameras |
| `McpServerManager.cpp` | Exposing the system to assistants over MCP |

## Inputs and outputs

Everything Calaos drives is an **IO**, derived from `IOBase` and created by `IOFactory` from the type declared in the configuration.

The generic types depend on no hardware: `InputSwitch`, `InputSwitchLongPress`, `InputSwitchTriple`, `InputAnalog`, `InputTemp`, `InputTime`, `InputTimer`, `InPlageHoraire`, `IntValue`, `OutputLight`, `OutputLightDimmer`, `OutputLightRGB`, `OutputShutter`, `OutputShutterSmart`, `OutputAnalog`, `OutputString`.

Hardware drivers live in their own subfolders: `Wago`, `KNX`, `Gpio`, `OneWire`, `Mqtt`, `Hue`, `OLA`, `MySensors`, `Reolink`, `LAN`, `Web`, `RemoteUI`. `ExternProc` additionally allows an IO to be delegated to an external process.

{{% notice tip %}}
**Adding a driver** means deriving `IOBase`, describing its parameters through `IODoc`, then registering it in `IOFactory`. `IODoc` feeds the built-in parameter documentation, which makes the new type immediately usable from Calaos Installer.
{{% /notice %}}

The `IO/io_types.txt` file lists the available types.

## Rules, scenarios and scripts

The rules engine ties **conditions** on input states to **actions** on outputs. Rules are described in `rules.xml` and reloaded every time a configuration is uploaded.

For what rules cannot express, `LuaScript/` embeds **LuaJIT**: a script receives the state of the system and acts on the IOs. See [Scripts]({{% relref "calaos_installer/scripts" %}}).

**Time ranges** and solar computations rely on `TimeRange`, `Calendar` and `sunset.c` — hence the importance of setting [latitude and longitude]({{% relref "calaos_os/configuration/date" %}}).

## The JSON API

This is the entry point for every client. It is available over two transports:

| Transport | Address |
|---|---|
| WebSocket | `ws://server:5454/api` |
| HTTP | `http://server:5454/api.php` |

WebSocket is preferable: it lets the server **push state changes** to the client, where HTTP forces periodic polling.

### How a session goes

1. **`login`** — authentication with the `cn_user` / `cn_pass` credentials.
2. **`get_home`** — fetching the structure: rooms, IOs, types, display parameters.
3. **`get_state`** / **`get_states`** — reading the current state of the IOs.
4. **`set_state`** — acting on an output.
5. The server then emits **`event`** messages on every change.

Each request carries a `msg` (the method name) and a `msg_id` echoed back in the response, which lets responses be matched to requests on a multiplexed connection.

### Main methods

| Area | Methods |
|---|---|
| Session | `login`, `change_cred` |
| State | `get_home`, `get_io`, `get_state`, `get_states`, `set_state`, `query` |
| Settings | `get_param`, `set_param`, `del_param` |
| Time ranges | `get_timerange`, `set_timerange` |
| Scenarios | `autoscenario` |
| History | `eventlog`, `get_stats` |
| Cameras | `get_camera_pic` |
| Audio | `get_playlists`, `get_playlist`, `get_artists`, `get_album`, `get_radios`, `audio_action` |
| Notifications | `register_push` |

`JsonApiHandlerWS.cpp` is the authoritative list; it is the reference to consult before writing a client.

## Discovery on the network

A client that does not know the server address broadcasts the message `CALAOS_DISCOVER` over UDP on **port 4545**. The server answers with its connection details.

This is the mechanism that lets Calaos Home, the mobile applications and the Remote UI screens find the server with no configuration. See `UDPServer.cpp`.

## Ports

| Port | Protocol | Use |
|---|---|---|
| 5454 | TCP | JSON API (HTTP and WebSocket), changed with the `port_api` setting |
| 4545 | UDP | Discovery |
| 4646 | UDP | Listening for Wago PLCs |

## Logging

Traces go through `Logger`, with **domains** allowing one subsystem to be targeted. They are controlled by `debug_enabled`, `debug_level` and `debug_domains` — see [Logs]({{% relref "calaos_os/configuration/logs" %}}).

In the code, one writes for example:

```cpp
cDebugDom("network") << "Got a CALAOS_DISCOVER";
```

## The companion tools

The same repository provides several utilities, in `src/bin/tools`:

| Tool | Role |
|---|---|
| `calaos_config` | Reading and writing settings — see [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}) |
| `calaos_mail` | Sending emails, used by notifications — see [Email]({{% relref "calaos_os/configuration/email" %}}) |
| `calaos_picture` | Processing camera pictures |
| `wago_test` | Testing and diagnosing Wago PLCs |

To these is added `src/bin/calaos_mcp`, which exposes the installation to **MCP**-compatible assistants.
