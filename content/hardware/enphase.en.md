+++
title = 'Enphase'
date = 2024-03-11T17:05:40Z
summary = 'Enphase Envoy Gateway for Photovoltaic Panels'
+++

## Introduction

The **Envoy** is the gateway of Enphase photovoltaic installations: it collects data from every micro-inverter and knows at any moment the output of the installation, the consumption of the house, and the balance between the two.

Calaos retrieves these values to display them, record their history, and above all **use them in rules**.

## What it is for

Driving loads from the output is what makes the integration interesting: rather than putting up with your production, you trigger consumption at the right moment.

- **start the water heater or the car charger** when output goes above a threshold;
- **postpone a washing machine** to hours of high production;
- **track self-consumption** with graphs over time;
- **be alerted** if production stops unexpectedly.

## The Envoy service

Calaos OS ships a dedicated service, which queries the Enphase gateway and makes the values available on the server.

Its configuration lives in `/mnt/calaos/envoy/envoy.toml`, created automatically on first boot from a template. The service exposes a local web interface on **port 8100**.

After any change to the file, restart the service:

```sh
systemctl restart envoy
```

And to check that it works:

```sh
systemctl status envoy
journalctl -u envoy
```

The log verbosity is set in the `[log]` section of the configuration file.

Like all additional services, it runs in a container and its data lives under `/mnt/calaos` — see [Services and modules]({{% relref "calaos_os/containers" %}}).

## Prerequisites

**The Envoy gateway must be reachable** from the Calaos server, and the photovoltaic installation must already work and report its data in the Enphase interface.

{{% notice tip %}}
Give the Envoy a **fixed IP address**, or reserve one on your router. A gateway that changes address makes all your measurements disappear at once, and the history stops with no apparent explanation.
{{% /notice %}}

## Getting the values into Calaos

The measurements exposed by the service are then declared as analog input IOs in Calaos Installer, pointing at the local service.

The principle is that of [Web IOs]({{% relref "hardware/webio" %}}): Calaos queries an address at regular intervals and extracts the value it cares about. Always start by looking at what the service actually returns before configuring:

```sh
curl http://127.0.0.1:8100/
```

You will then know which values are available and in what form.

## Recording production history

Enable **value logging** on these IOs to build production and consumption curves over time. The measurements are then sent to the history database and can be plotted as graphs.

See [Create IOs]({{% relref "calaos_installer/io" %}}) for that option, and [Services and modules]({{% relref "calaos_os/containers" %}}) for the history database.

{{% notice note %}}
Production data changes continuously: there is no point querying it every second. A reading every few minutes is more than enough for readable curves, and lightens the load on the gateway.
{{% /notice %}}

## Driving loads from production

Once production is available as an analog input, it is used as a rule condition with the comparison operators:

> If production `>` 2000 **and** the water heater is off, then switch the water heater on.

Remember to avoid constant flapping around the threshold: an [internal variable]({{% relref "calaos_installer/internal_var" %}}) remembering that the appliance has already started, or a [timer]({{% relref "calaos_installer/time" %}}) enforcing a minimum running time, stops a passing cloud from clacking a relay every minute.

See [Create rules]({{% relref "calaos_installer/rules" %}}).

## Diagnosis

1. **is the service running?** `systemctl status envoy`
2. **does it answer** on `http://127.0.0.1:8100/`?
3. **is the Envoy gateway reachable** from the server?
4. **are the Enphase credentials** still valid?

See [Logs]({{% relref "calaos_os/configuration/logs" %}}).
