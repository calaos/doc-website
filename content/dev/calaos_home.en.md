+++
title = 'Calaos Home'
date = 2024-03-06T20:17:58Z
weight = 20
summary = "The local touchscreen interface, and how it drives the system."
+++

## Role

Calaos Home is the interface displayed on the **touchscreen connected to the server**. It presents the home — rooms, lights, shutters, temperatures, cameras, audio — and is also used for a few operations on the system itself: network configuration, installation onto the disk, confirming a rollback.

It is written in **Qt / QML** and lives in the [calaos_mobile](https://github.com/calaos/calaos_mobile) repository, which it shares with the mobile applications.

{{% notice info %}}
The repository is called `calaos_mobile` but contains **two products**: the desktop application for touchscreens (Calaos Home) and the iOS/Android applications. The desktop part is built with the `CALAOS_DESKTOP` flag.
{{% /notice %}}

## Code layout

| Location | Contents |
|---|---|
| `qml/desktop/` | Calaos Home interface |
| `qml/mobile/` | Mobile applications interface |
| `qml/SharedComponents/` | Components common to both |
| `src/` | C++ logic exposed to QML |

On the C++ side, the main classes are shared by both products:

| Class | Role |
|---|---|
| `CalaosConnection` | API client: WebSocket, HTTP, reconnection |
| `HomeModel`, `RoomModel`, `RoomFilterModel` | Models of the home exposed to QML |
| `AudioModel`, `CameraModel`, `EventLogModel` | Specialised models |
| `FavoritesModel`, `BookmarkModel`, `CalaosWidgetModel` | Interface personalisation |
| `HardwareUtils` | Platform abstraction |

And some exist only for the desktop version:

| Class | Role |
|---|---|
| `HardwareUtils_desktop` | Server discovery, cursor, local options |
| `ScreenManager` | Screen sleep |
| `XUtils` | Access to X11, in particular the DPMS extension |
| `CalaosOsAPI` | Driving the Calaos OS system |
| `OSInstaller`, `UsbDisk` | Installing the system onto a disk |

Interface state is managed with **QuickFlux**, an implementation of the Flux pattern for QML: components emit actions, which are handled centrally.

## Connecting to the server

At startup the application broadcasts `CALAOS_DISCOVER` over UDP on port 4545 and waits for the server to answer. The search is retried every five seconds as long as no server has replied.

If the `calaos_server_host` setting is filled in, **discovery is disabled** and the given address is used directly. See [Touchscreen]({{% relref "calaos_os/configuration/touchscreen" %}}).

`CalaosConnection` then builds the URL according to what it is given:

- an address starting with `ws://` or `wss://` is used as is;
- an address starting with `http://` or `https://` is converted to its WebSocket equivalent;
- a plain IP address gives `ws://address:5454/api` on the desktop version.

## Local settings

The application reads its own configuration from `local_config.xml`, using the same keys as those described in [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}): `show_cursor`, `dpms_enable`, `dpms_standby`, `calaos_server_host`, `cn_user`, `cn_pass`, `latitude`, `longitude`.

On first launch, if no file exists, the application writes one with default values: credentials `user` / `pass`, cursor shown, sleep disabled, and geographic coordinates pointing at Paris.

`ScreenManager` reads `dpms_standby` in minutes and converts it to milliseconds; with no usable value it falls back to one minute.

## Driving the system

This is what makes Calaos Home special: beyond home automation, it administers the machine. It does so through a **local REST API**, exposed by the `calaos-container` daemon on `http://127.0.0.1:8000`.

Authentication uses a token, read from `/run/calaos/calaos-ct.token` — a file regenerated at every boot by `init_calaosfs` — and passed in an `Authorization: bearer <token>` header.

| Endpoint | Use |
|---|---|
| `GET /api/system/info` | System information |
| `GET /api/system/fs_status` | Filesystem status |
| `POST /api/system/reboot` | Reboot the machine |
| `POST /api/system/restart` | Restart the services |
| `POST /api/system/rollback_snapshot` | Confirm a rollback |
| `GET /api/system/install/list_devices` | List the available disks |
| `POST /api/system/install/start` | Start the installation |
| `GET /api/system/install/status` | Installation progress |
| `GET /api/network/list` | List the network interfaces |
| `POST /api/network/<interface>` | Configure an interface |

These endpoints explain behaviours described elsewhere in this documentation: the confirmation button that appears when you have booted on a restore point — see [Rolling back]({{% relref "calaos_os/rollback" %}}) — and the disk installation offered from Live mode, described in [Installation]({{% relref "calaos_os/installation" %}}).

{{% notice warning %}}
This API only listens on the loopback interface. It is not reachable from the network, and is not meant to be consumed by anything other than the local interface.
{{% /notice %}}

## Building

The project is built with Qt. The repository's GitHub Actions workflows (`.github/workflows/`) describe the full chain, including the Docker build images, and are the most reliable reference for reproducing a build.
