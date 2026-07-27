+++
title = 'Audio/Video Receivers'
date = 2024-03-06T20:05:32Z
weight = 80
summary = 'Audio Video Receiver using Ethernet port (Denon, Marantz, Yamaha, ...)'
+++

## Introduction

Calaos drives **home cinema receivers** connected to the network: switching them on and off, setting the volume, changing the source. The receiver becomes a device like any other, usable in your rules and scenarios.

This is what lets a "Cinema" scenario switch the receiver on to the right input at the same time as it dims the lights and closes the shutters.

## Supported brands

| Brand | Identifier |
|---|---|
| Denon | `denon` |
| Marantz | `marantz` |
| Onkyo | `onkyo` |
| Pioneer | `pioneer` |
| Yamaha | `yamaha` |
| HiFi Rose | `hifirose` |

These manufacturers all expose a network control protocol, which Calaos uses directly. No gateway or extra box is needed.

## Prerequisites

**The receiver must be connected to the network**, preferably over Ethernet, and reachable from the Calaos server.

{{% notice tip %}}
Give it a **fixed IP address**, or reserve one on your router. A receiver that changes address after an outage becomes unreachable, and the symptom — "commands no longer get through" — does not naturally point at the network.
{{% /notice %}}

Also check in the device settings that **network control is enabled**. Many receivers disable it by default, or cut it in standby to save power: in the latter case, Calaos will not be able to switch them back on.

## Parameters

| Name | Description |
|---|---|
| `host` | IP address or name of the receiver on the network |
| `model` | Brand of the device, from the list above |
| `output` | Output zone of the receiver, for multi-zone models |

## The zone parameter

Many receivers drive **several zones**: the main room and one or two secondary zones feeding other rooms.

Each zone is declared separately in Calaos, with the same address but a different output. You then get independent control of the living room and the terrace, from the same device.

## Using it in rules

Once declared, the receiver is used like any IO:

- **in a scenario** — "Cinema" switches the receiver on to the projector input;
- **as a rule action** — switch the receiver off when the "Leaving" scenario triggers;
- **as a condition** — only switch the living room lights off if the receiver is running.

See [Create rules]({{% relref "calaos_installer/rules" %}}) and [Scenario]({{% relref "calaos_installer/scenario" %}}).

## Diagnosis

If the receiver does not answer:

1. **does it answer a ping** from the Calaos server?
2. **is network control enabled** in its settings?
3. **does it stay active in standby?** If not, the receiver can never be switched back on remotely.
4. **has its address changed** since it was configured?

See [Logs]({{% relref "calaos_os/configuration/logs" %}}) for communication errors.

## See also

For multi-room music playback, see [Music zones]({{% relref "hardware/music" %}}).
