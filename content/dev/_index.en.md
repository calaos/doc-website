+++
title = 'Software stack'
date = 2024-03-06T20:19:42Z
weight = 100
chapter = true
+++

### For developers

# Calaos software stack

This chapter describes how Calaos works internally, component by component. It is aimed at developers, contributors and advanced users who want to understand how the pieces fit together, write a client, or port Calaos to new hardware.

{{% notice info %}}
**Nothing in this chapter is needed to use Calaos.** To install and configure your system, see [Getting started]({{% relref "getting_started" %}}) and [Calaos OS]({{% relref "calaos_os" %}}).
{{% /notice %}}

## Overview

The whole installation revolves around a single process, **calaos_server**. It talks to the hardware, applies the rules and exposes an API. Every other component — graphical interfaces, wall screens, mobile applications — is a **client of that API**, with no direct access to the hardware.

{{< mermaid >}}
graph TD
    subgraph clients[Clients]
        HOME[Calaos Home<br/>local touchscreen]
        MOBILE[Calaos Mobile<br/>iOS / Android]
        WEB[Calaos WebApp<br/>browser]
        RUI[Remote UI<br/>ESP32 wall screens]
        INST[Calaos Installer<br/>configuration]
    end

    subgraph server[Server]
        SRV[calaos_server]
        RULES[Rules and<br/>scenario engine]
        IO[Input / output drivers]
    end

    subgraph hw[Hardware and services]
        WAGO[Wago / KNX / GPIO]
        MQTT[MQTT / Zigbee]
        ONEWIRE[1-Wire / DMX / Hue]
        DB[(InfluxDB<br/>history)]
    end

    HOME -->|JSON / WebSocket| SRV
    MOBILE -->|JSON / WebSocket| SRV
    WEB -->|JSON / WebSocket| SRV
    RUI -->|JSON / WebSocket| SRV
    INST -->|JSON / HTTP| SRV

    SRV --- RULES
    SRV --- IO
    IO --> WAGO
    IO --> MQTT
    IO --> ONEWIRE
    SRV --> DB
{{< /mermaid >}}

This architecture has one practical consequence: **writing a new client requires touching nothing else**. The API is the same for everyone, documented by the `calaos_server` code, and the existing clients are so many examples of it.

## The components

| Component | Role | Technology | Repository |
|---|---|---|---|
| [Calaos Server]({{% relref "dev/calaos_server" %}}) | Core of the system: hardware, rules, API | C++ / libuv | [calaos_base](https://github.com/calaos/calaos_base) |
| [Calaos Home]({{% relref "dev/calaos_home" %}}) | Local touchscreen interface | Qt / QML | [calaos_mobile](https://github.com/calaos/calaos_mobile) |
| [Calaos Mobile]({{% relref "dev/calaos_mobile" %}}) | iOS and Android applications | Qt / QML | [calaos_mobile](https://github.com/calaos/calaos_mobile) |
| [Remote UI]({{% relref "dev/calaos_remoteui" %}}) | Standalone wall screens | ESP32-P4 | [calaos_remote_ui](https://github.com/calaos/calaos_remote_ui) |
| [Calaos WebApp]({{% relref "dev/calaos_webapp" %}}) | Browser interface | JavaScript | [calaos-web-app](https://github.com/calaos/calaos-web-app) |
| [Calaos DDNS]({{% relref "dev/calaos_ddns" %}}) | Remote access and certificates | Go | [calaos_ddns](https://github.com/calaos/calaos_ddns) |
| [Configuration files]({{% relref "dev/calaos_config" %}}) | Format and location of the configuration | XML | [calaos_base](https://github.com/calaos/calaos_base) |

On top of these come the components that run the system itself: **calaos-container**, the daemon managing the containerised services, and **calaos-os**, the administration tool. They are described in [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}) and [The calaos-os tool]({{% relref "calaos_os/advanced/cli" %}}).

## Network ports

| Port | Protocol | Use |
|---|---|---|
| 5454 | TCP | JSON API, over HTTP and WebSocket |
| 4545 | UDP | Server discovery on the local network |
| 4646 | UDP | Listening for Wago PLCs |
| 8000 | TCP | `calaos-container` administration API, **on the loopback interface only** |

The API port can be changed with the `port_api` setting.

The API on port 8000 is the one Calaos Home uses to install the system, configure the network and confirm a rollback — see [Calaos Home]({{% relref "dev/calaos_home" %}}).

## Licence

Calaos is free software, released under **GPL v3** for most of its components. Contributions are welcome: every repository accepts pull requests, and discussions happen on the forum and in GitHub issues.

{{% children description="true" %}}
