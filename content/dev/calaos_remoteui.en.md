+++
title = 'Calaos Remote UI'
date = 2026-03-06T20:17:52Z
weight = 35
summary = "The wall screen firmware, and how to port it to new boards."
+++

## Role

Remote UI is the firmware of the **standalone wall screens**: small touch panels flush-mounted around the house, which connect to the server over the network and display an interface configured from Calaos Installer.

Unlike [Calaos Home]({{% relref "dev/calaos_home" %}}), which runs on the server itself, a Remote UI screen is an independent device, separately powered, with no general-purpose operating system.

Repository: [calaos_remote_ui](https://github.com/calaos/calaos_remote_ui), written in **C**.

{{% notice info %}}
For installing, wiring and pairing a screen, see the hardware page [Remote UI]({{% relref "hardware/remote_ui" %}}). This page covers the firmware itself.
{{% /notice %}}

## Platform

The firmware targets the **ESP32-P4** microcontroller and is built with the ESP-IDF environment, driven by CMake. The `sdkconfig.defaults` and `sdkconfig.defaults.esp32p4` files provide the base configuration.

## Supported boards

The `boards/` folder holds one definition per board, each with its own CMake file:

| Board | Form factor |
|---|---|
| `waveshare-86-panel` | 86 mm wall panel |
| `luckfox-86-panel` | 86 mm wall panel |
| `waveshare-touchlcd-7` | 7-inch touchscreen |
| `waveshare-touchlcd-8` | 8-inch touchscreen |
| `waveshare-touchlcd-10` | 10-inch touchscreen |
| `linux-dev` | Development target, on a computer |

**Porting the firmware to a new board** therefore means adding a definition in `boards/` and describing its display, its touch panel and its networking, without touching the application logic. The `boards/ci-boards.json` file lists the boards built automatically by continuous integration.

## Code layout

| Location | Contents |
|---|---|
| `main/` | Firmware entry point |
| `hal/` | Hardware abstraction layer |
| `network/` | Network connection and dialogue with the server |
| `flux/` | Interface state management |
| `components/` | ESP-IDF components |
| `smooth_ui_toolkit/` | Interface library |
| `fonts/` | Embedded fonts |
| `boards/` | Board definitions |
| `patches/` | Patches applied to dependencies |
| `docs/` | Internal documentation, including `BOARD.md` |

## Developing without hardware

The `linux-dev` target, with the `build_linux.sh` script, lets you **build and run the interface on a computer**. It is the fastest way to iterate on the display without reflashing a board for every attempt.

For everything hardware-related — display, touch, networking — testing on a real board remains essential, the `hal/` layer being precisely what this target simulates.

## Life cycle of a screen

From the firmware's point of view, commissioning goes like this:

1. **Network connection** — Wi-Fi or Ethernet, according to what was written into the firmware at flash time.
2. **Time synchronisation** over NTP. This step is **blocking**: authentication with the server relies on the clock, and a screen with the wrong time will not connect.
3. **Server discovery** on the local network, or connection to the configured address.
4. **Pairing** — the screen generates and displays a code, which must be declared in Calaos Installer. Until the server knows the code, the connection is refused and the screen shows the matching prompt.
5. **Normal operation** — the screen receives its interface configuration and state changes from the server.

On the server side this maps to a dedicated IO type: see `IO/RemoteUI/` and `RemoteUI/` in [Calaos Server]({{% relref "dev/calaos_server" %}}).

## Updates

Flashing over USB is only needed once, at commissioning. Later updates happen **over the network (OTA)**.

This is also why the `ota` domain appears among those that can be enabled in the server's diagnostic logs — see [Logs]({{% relref "calaos_os/configuration/logs" %}}):

```sh
calaos_config set debug_domains ota:5,remote_ui:5,remoteui:5
```
