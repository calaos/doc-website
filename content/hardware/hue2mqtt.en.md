+++
title = 'Hue2mqtt'
date = 2024-03-11T17:05:29Z
summary = 'Philips Hue Lamps via MQTT'
+++

## Introduction

**Philips Hue** bulbs and light strips can be driven from Calaos: switching, dimming, colour. They appear in the interfaces and are used in rules like any other lighting.

Calaos goes through the **Hue bridge**, the box supplied by Philips to which the bulbs are paired. The bulbs themselves are never addressed directly.

## Two ways of doing it

| Method | Principle | Best when |
|---|---|---|
| **Direct Hue IO** | Calaos queries the Hue bridge | You only drive lamps |
| **hue2mqtt gateway** | A service translates Hue into MQTT | You already use MQTT, or want to report other Hue devices |

The first is the simplest to set up. The second fits better into an installation where MQTT is already the common meeting point — see [MQTT]({{% relref "hardware/mqtt" %}}).

## Method 1 — Direct Hue IO

Calaos Installer offers an output type for an **RGB light driven by a Hue bridge**.

| Name | Type | Required | Description |
|---|---|---|---|
| `host` | string | yes | IP address of the Hue bridge |
| `api` | string | yes | API key returned by the bridge when the association is made |
| `id_hue` | string | yes | Unique identifier of the lamp on the bridge |

{{% notice tip %}}
Do not try to obtain the API key and identifiers by hand: **Calaos Installer includes a Hue wizard** that fetches them automatically. You simply have to press the button on the bridge when it asks, as for any Hue association.
{{% /notice %}}

## Method 2 — The hue2mqtt gateway

`hue2mqtt` is a service that connects to the Hue bridge and publishes everything it sees onto MQTT. Calaos then consumes those messages with ordinary MQTT IOs.

It ships with Calaos OS and is configured in `/mnt/calaos/hue2mqtt`. The configuration file has two sections:

```toml
[mqtt]
host = "127.0.0.1"
port = 1883
topic_prefix = "hue2mqtt"

[hue]
ip = "192.168.1.25"
username = "the key supplied by the bridge"
```

The `[mqtt]` section states where to publish — the local Mosquitto broker by default — and the `[hue]` section how to reach the bridge.

The service also accepts MQTT authentication and TLS encryption, if your broker requires them.

After a change, restart the service:

```sh
systemctl restart hue2mqtt
```

The IOs are then created as MQTT IOs, subscribing to the topics published under the configured prefix. To discover those topics, a tool such as MQTTX lets you watch what actually flows — see [MQTT]({{% relref "hardware/mqtt" %}}).

## Common prerequisites

**The Hue bridge must be reachable** from the Calaos server, and preferably at a fixed address.

The bulbs must already be **paired with the bridge** through the Philips Hue application. Calaos drives them, it does not pair them.

{{% notice note %}}
A Hue bulb cut off by a wall switch becomes unreachable: this is the normal behaviour of these products, which must stay powered at all times. If your lamps regularly disappear, check this first before looking at the configuration.
{{% /notice %}}

## Diagnosis

1. **does the bridge answer** from the Calaos server?
2. **is the API key still valid?** Resetting the bridge invalidates it.
3. **is the bulb powered** and visible in the Philips Hue application?
4. With hue2mqtt: **are messages arriving** on the MQTT broker?

See [Logs]({{% relref "calaos_os/configuration/logs" %}}), and [Services]({{% relref "calaos_os/configuration/services" %}}) for the state of the `hue2mqtt` service.
