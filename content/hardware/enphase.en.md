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

## Why a dedicated service

Since an Enphase firmware update in 2022, **the gateway can no longer be queried directly** on the local network. You must first authenticate with the Enlighten online service, obtain a token valid for your gateway, and only then query the Envoy at home.

This dance is out of reach of a simple HTTP request. Calaos OS therefore ships a dedicated service that takes care of it: it handles authentication, renews the token on its own, queries the gateway and **makes the data available in a directly usable form**.

The code is available on [go-envoy](https://github.com/raoulh/go-envoy).

## Setting up access

Configuration takes a single command, from the server over SSH. Have three pieces of information at hand: your **Enlighten account** (the one used by the Enphase application) and the **serial number of your gateway**, printed on the device and visible in the application.

```sh
envoy config set -h 192.168.1.134 -u my.account@example.com -s 1234567890 -p my_password
```

| Option | Meaning |
|---|---|
| `-h` | IP address of the gateway on your network |
| `-u` | Enlighten account login |
| `-s` | Serial number of the gateway |
| `-p` | Enlighten account password |

{{% notice tip %}}
The `-h` option is optional: without it the service **finds the gateway on its own** on the local network. Fill it in if discovery fails, or if you have several gateways.
{{% /notice %}}

This information is stored along with the authentication token in the service's data folder, under `/mnt/calaos/envoy`. It is therefore part of your data and survives updates — see [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

## Checking that it works

The quickest way to validate the configuration:

```sh
envoy now
```

which shows instantaneous production and consumption:

```text
🔌Production: 59.43W / 2354W    Consumption: 1689.83W   Net import: 1630.40W
```

If this command answers, authentication and access to the gateway are fine. Other commands go further:

| Command | Effect |
|---|---|
| `envoy now` | Instantaneous production and consumption |
| `envoy today` | Statistics for the day |
| `envoy info` | Information about the gateway |
| `envoy production` | Raw production data, as JSON |
| `envoy inventory` | Inventory of the equipment |
| `envoy inverters` | State of each micro-inverter |

## The service

The daemon queries the gateway continuously, caches the data and exposes it on **port 8100**.

```sh
systemctl status envoy
systemctl restart envoy
journalctl -u envoy
```

Its general configuration — port, log level — lives in `/mnt/calaos/envoy/envoy.toml`. Log verbosity is set in the `[log]` section.

Like all additional services, it runs in a container — see [Services and modules]({{% relref "calaos_os/containers" %}}).

## Getting the values into Calaos

The service exposes three JSON endpoints:

| Address | Contents |
|---|---|
| `http://127.0.0.1:8100/api/production` | Production and consumption |
| `http://127.0.0.1:8100/api/inventory` | Inventory of the equipment |
| `http://127.0.0.1:8100/api/inverters` | State of each micro-inverter |

A web interface is also available on `http://server-address:8100/`.

These values are then declared in Calaos Installer as **analog inputs of the Web IO type**, which query the service and extract the value you want — see [Web IO]({{% relref "hardware/webio" %}}).

First look at what the endpoint returns to work out the path to use:

```sh
curl http://127.0.0.1:8100/api/production
```

The response holds two lists, `production` and `consumption`, each made of measurements. The most useful fields are:

| Field | Meaning |
|---|---|
| `wNow` | Instantaneous power, in watts |
| `whToday` | Energy accumulated since the start of the day, in watt-hours |
| `whLifetime` | Energy accumulated since installation |

{{% notice note %}}
Production data changes continuously: there is no point querying it every second. A reading every few minutes is more than enough for readable curves, and lightens the load on the gateway.
{{% /notice %}}

## Recording production history

Enable **value logging** on these IOs to build production and consumption curves over time. See [Create IOs]({{% relref "calaos_installer/io" %}}).

## Driving loads from production

Once production is available as an analog input, it is used as a rule condition:

> If production `>` 2000 **and** the water heater is off, then switch the water heater on.

Remember to avoid constant flapping around the threshold: an [internal variable]({{% relref "calaos_installer/internal_var" %}}) remembering that the appliance has already started, or a [timer]({{% relref "calaos_installer/time" %}}) enforcing a minimum running time, stops a passing cloud from clacking a relay every minute.

See [Create rules]({{% relref "calaos_installer/rules" %}}).

## Diagnosis

1. **does `envoy now` answer?** This is the most direct test: if it fails, the problem is in the configuration or in reaching the gateway, not in Calaos.
2. **is the service running?** `systemctl status envoy`
3. **does the endpoint answer?** `curl http://127.0.0.1:8100/api/production`
4. **has the Enlighten password changed?** Authentication goes through Enphase's online service: a password change interrupts collection.

{{% notice warning %}}
The service needs **Internet access** to renew its token with Enphase. A prolonged connection outage therefore ends up interrupting data collection, even if the gateway remains reachable on the local network.
{{% /notice %}}

See [Logs]({{% relref "calaos_os/configuration/logs" %}}).
