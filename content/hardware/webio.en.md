+++
title = 'Web IO'
date = 2024-03-06T20:05:03Z
weight = 70
summary = 'Control devices using Web API calls'
+++

## Introduction

**Web IOs** let you drive or read any device that exposes a **web interface**: a REST API, a status page, a JSON or XML file served over HTTP.

This is the universal fallback. When a device has no dedicated driver in Calaos but is reachable over the network, there is a good chance of integrating it this way without writing a line of code.

A few typical uses:

- reading the output of a solar inverter or a smart meter;
- fetching weather data from an online service;
- controlling a connected socket or a relay module that exposes an API;
- querying a device on the local network that publishes its state as JSON.

## How it works

Calaos queries an **address** at regular intervals, then extracts the value it cares about from the response.

Extraction relies on a **path**: in a JSON or XML document, it points to where the data is. If your device answers, for example:

```json
{ "sensors": { "outdoor": { "temp": 12.4, "humidity": 68 } } }
```

the path leads to `temp` inside `sensors` then `outdoor`.

For outputs the principle is symmetrical: Calaos sends a request to the given address to pass on the new value.

## Supported IO types

**As inputs** — temperature sensor, analog input, text input.

**As outputs** — light, RGB light, analog output, text output.

## Parameters

| Name | Description |
|---|---|
| `url` | Address to query or call |
| `path` | Location of the value inside the received document |
| `file_type` | Format of the document: JSON, XML, plain text |
| `request_type` | HTTP request method |
| `data` | Data to send, for outputs |
| `data_type` | Format of that data |

{{% notice note %}}
These IOs are among the most flexible in Calaos, and therefore among the most dependent on the target device. **The built-in help in Calaos Installer** shows the exact description of every parameter for the chosen type — see [Create IOs]({{% relref "calaos_installer/io" %}}).
{{% /notice %}}

## Before configuring

Always start by **looking at what the device returns**. Open its address in a browser, or from the server:

```sh
curl http://device-address/api/status
```

You then see the real structure of the response, and can work out the path to fill in. Without this step, configuration is done blind.

## Choosing between Web IO and MQTT

Both allow integrating a device that is not natively supported, but they are not equivalent in every case.

| | Web IO | MQTT |
|---|---|---|
| **How it works** | Calaos queries the device | The device publishes its changes |
| **Responsiveness** | Depends on the polling interval | Immediate |
| **Best for** | Data that changes slowly | A state that must be reported at once |

For an outdoor temperature or solar output, periodic polling is perfectly fine. For an opening detector, MQTT is clearly preferable: see [MQTT]({{% relref "hardware/mqtt" %}}).

{{% notice warning %}}
Avoid querying an **online service** too often, or you risk being rate-limited or blocked. For weather data, a few minutes between requests is more than enough.
{{% /notice %}}

## Diagnosis

If no value comes back:

1. **does the address answer** from the Calaos server, using `curl`?
2. **does the declared format** match what the device actually returns?
3. **does the path** match the structure of the response? A misspelled key returns an empty value.
4. **does the device require authentication** that the request does not provide?

See [Logs]({{% relref "calaos_os/configuration/logs" %}}) to follow the exchanges on the server side.
