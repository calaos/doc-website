+++
title = 'Calaos DDNS'
date = 2024-03-06T20:18:24Z
weight = 50
summary = "The dynamic DNS client, certificates and HAProxy routing."
+++

## Role

Calaos DDNS solves a remote access problem: the public address of a home connection changes regularly, which breaks any configuration relying on a fixed address.

The client runs on the server, registers a `.calaos.fr` name with the Calaos service, and keeps it up to date. Along the way it obtains a **Let's Encrypt certificate** and generates the **HAProxy** configuration that routes incoming connections.

| Component | Repository | Role |
|---|---|---|
| Client | [calaos_ddns](https://github.com/calaos/calaos_ddns) | Runs on the user's server |
| Service | [calaos_dns](https://github.com/calaos/calaos_dns) | Hosted by Calaos, holds the DNS zone |

Both are written in **Go**. The reference service is reachable at `https://ns1.calaos.fr/`.

{{% notice info %}}
For everyday usage — registering a domain, opening a port, diagnosing — see [Dynamic DNS]({{% relref "calaos_os/configuration/calaos_ddns" %}}).
{{% /notice %}}

## Commands

| Command | Effect |
|---|---|
| `calaos_ddns register DOMAIN [SUBDOMAIN...]` | Registers a name and requests a certificate |
| `calaos_ddns update [-f]` | Updates the address; `-f` forces certificate renewal |
| `calaos_ddns unregister` | Removes the registration from the service |
| `calaos_ddns reset` | Clears the local token |

## The token

The whole relationship with the service rests on a **token** obtained at registration, then stored in the server configuration under the `ddns_token` key. The email address used for Let's Encrypt is kept under `ddns_le_email`.

This explains a behaviour that can be confusing: `unregister` starts by presenting the token to the service in order to delete the registration, and **only cleans the local configuration if the service accepts**. If the token is no longer valid, the command fails without clearing anything, and you end up stuck.

That is what `reset` is for; it simply deletes the key locally — the exact equivalent of:

```sh
calaos_config del ddns_token
```

## Subdomains and routing

A subdomain argument accepts two forms:

```sh
calaos_ddns register myhome camera1
calaos_ddns register myhome camera1=192.168.0.20:4444
```

The first simply creates `camera1.myhome.calaos.fr`. The second additionally adds an **HAProxy entry** directing connections to the given device on the local network.

The HAProxy configuration is generated in `/mnt/calaos/haproxy`, and the certificate written to `server.pem`. These paths are given to the client through environment variables set in its systemd unit:

| Variable | Value |
|---|---|
| `CALAOS_CONFIG` | `/mnt/calaos/config` |
| `CALAOS_HAPROXY_PATH` | `/mnt/calaos/haproxy` |
| `CALAOS_CERT_FILE` | `/mnt/calaos/haproxy/server.pem` |
| `CALAOS_HAPROXY_TEMPLATE_PATH` | `/usr/share/calaos-ddns` |
| `CALAOSDNS_CACHE_DIR` | `/mnt/calaos/calaos-ddns` |

As everything sits under `/mnt/calaos`, the configuration and the certificate are part of the user's data: they survive updates and are covered by a backup of that folder.

## Periodic update

The client is not a permanent service: it is a one-shot task, triggered by a systemd timer.

```ini
[Timer]
OnBootSec=1min
OnUnitActiveSec=2h
AccuracySec=30min
```

That is a first check one minute after boot, then every two hours. On each run the client compares the public address with the registered one, renews the certificate if it is nearing expiry, and restarts HAProxy when the configuration has changed.

To watch it work:

```sh
systemctl status calaos-ddns.timer
journalctl -u calaos-ddns.service
```
