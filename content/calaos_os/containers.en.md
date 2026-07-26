+++
title = 'Services and modules'
date = 2026-07-25T10:00:00Z
weight = 50
summary = "The additional software shipped with Calaos OS."
+++

## What this is about

Calaos OS does not contain only the Calaos server. It also ships a series of additional pieces of software, each bringing one function: talking to Zigbee devices, recording the history of your measurements, drawing graphs, driving DMX lighting…

Each one runs **isolated from the others**, in what is called a container. Concretely, this means that:

- a module that crashes does not take the rest of your installation with it;
- each one updates independently;
- each one stores its data in its own folder.

You have nothing to install: they are already there. Some are only useful if you own the matching hardware.

## What is included

| Service | What it does | Documentation |
|---|---|---|
| Mosquitto | Carries MQTT messages between your devices and Calaos | [MQTT]({{% relref "hardware/mqtt" %}}) |
| Zigbee2MQTT | Bridges your Zigbee devices and Calaos | [Zigbee]({{% relref "hardware/zigbee" %}}) |
| InfluxDB | Records the history of your measurements: temperatures, consumption… | — |
| Grafana | Displays those measurements as graphs | — |
| OLA | Drives DMX lighting | [DMX / OLA]({{% relref "hardware/dmx_ola" %}}) |
| hue2mqtt | Bridges Philips Hue bulbs | [Hue]({{% relref "hardware/hue2mqtt" %}}) |
| Envoy | Retrieves data from Enphase solar inverters | [Enphase]({{% relref "hardware/enphase" %}}) |
| HAProxy | Routes incoming network connections to the right service | — |

## See what is running

Connect to the server over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})), then:

```sh
podman ps
```

The list shows the active services, with their name and how long they have been running.

To know the installed versions and whether updates exist:

```sh
calaos-os list
```

## Updating

The additional services are updated together with the rest of the system:

```sh
calaos-os upgrade
```

See [Update]({{% relref "calaos_os/update" %}}) for the complete procedure.

To update one specific service without touching the rest:

```sh
calaos-os upgrade SERVICE_NAME
```

## Where your data is stored

Each service stores its data in its own subfolder of `/mnt/calaos`:

```text
/mnt/calaos/
├── config/          Calaos settings
├── influxdb/        history of your measurements
├── grafana/         graphs and dashboards
├── mosquitto/       MQTT data
├── zigbee2mqtt/     paired Zigbee devices
├── ola/             DMX configuration
├── hue2mqtt/        Hue configuration
├── envoy/           Enphase configuration
├── haproxy/         network configuration
└── xorg.conf.d/     display settings
```

Backing up this folder backs up everything: see [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

{{% notice info %}}
The `zigbee2mqtt` folder deserves particular attention: it holds the list of your paired Zigbee devices. Losing it would force you to pair them all again, one by one.
{{% /notice %}}

## When a service stops working

First check its logs, which almost always show the cause:

```sh
podman logs SERVICE_NAME
```

See also [Logs]({{% relref "calaos_os/configuration/logs" %}}) and [Services]({{% relref "calaos_os/configuration/services" %}}).

## Going further

The services run with **Podman**, an alternative to Docker that needs no central daemon to work.

The service images are described by `.source` files stored in `/usr/share/calaos/`. During installation they are exported into a local cache, in `/var/lib/cache/containers`: this is what lets your server boot for the first time **without Internet access**.
