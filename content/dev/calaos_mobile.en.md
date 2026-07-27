+++
title = 'Calaos Mobile'
date = 2024-03-06T20:18:02Z
weight = 30
summary = "The iOS and Android applications, and the base they share with Calaos Home."
+++

## Role

Calaos Mobile is the application that drives your installation from a phone or a tablet, at home as well as away. It is available on **iOS** and **Android**.

Repository: [calaos_mobile](https://github.com/calaos/calaos_mobile), written in **Qt / QML**.

## One repository, two products

The same repository produces the mobile application and [Calaos Home]({{% relref "dev/calaos_home" %}}), the desktop interface for touchscreens. Both share most of their logic:

```text
qml/
├── mobile/            mobile applications interface
├── desktop/           Calaos Home interface
├── SharedComponents/  common components
└── quickflux/         state management

src/                   shared C++ logic, exposed to QML
```

This sharing has a useful consequence: **a fix in the models or in the API client benefits both products**. What diverges is limited to presentation and platform-specific features.

## The API client

`CalaosConnection` is the central class: it opens the connection, authenticates the session, issues requests and dispatches incoming events.

It accepts several address forms and picks the transport accordingly:

| Address given | Connection established |
|---|---|
| `ws://` or `wss://` | WebSocket, as is |
| `http://` or `https://` | Converted to the equivalent WebSocket |
| Plain address, mobile version | `wss://address/api` |
| Plain address, desktop version | `ws://address:5454/api` |

The mobile version therefore favours **an encrypted connection**, which matches its usage: it is often used from outside, across the Internet. This is where [dynamic DNS]({{% relref "dev/calaos_ddns" %}}) and its certificate make full sense.

On the local network the application can also discover the server automatically, through the same UDP broadcast mechanism as the other clients.

## The models exposed to QML

The interface does not handle the API's raw JSON: the C++ layer translates it into Qt models, which QML consumes.

| Model | Contents |
|---|---|
| `HomeModel` | Complete structure of the home |
| `RoomModel`, `RoomFilterModel` | Rooms, and filtering by type |
| `AudioModel` | Players, playlists, radios |
| `CameraModel` | Cameras |
| `EventLogModel` | Event history |
| `FavoritesModel`, `BookmarkModel` | User shortcuts |
| `CalaosWidgetModel` | Configurable widgets |
| `UserInfoModel` | Session |
| `WeatherInfo` | Weather |

## Push notifications

The application registers its token with the server through the API's `register_push` method, stating the platform. The server can then warn it: alarm triggered, device disconnected, battery low.

The automatic device-related notifications are configured on the server side — see the `notif/*` keys in [Email]({{% relref "calaos_os/configuration/email" %}}).

## Building

The repository provides GitHub Actions workflows covering the build and release for each platform, including the Docker build images. They are the reference to follow when reproducing a build environment, as the iOS and Android toolchains each have their own constraints.

The repository also holds the `android/` and `ios/` folders, carrying the platform-specific files, along with a privacy policy (`PRIVACY_POLICY.md`) required by the application stores.
