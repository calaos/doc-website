+++
title = 'KNX'
date = 2024-03-06T20:04:17Z
weight = 20
summary = 'KNX devices using knxd daemon'
+++

## Introduction

KNX is a very widespread home automation bus standard, particularly in commercial buildings and in installations carried out by integrators. Calaos can drive an existing KNX bus, which lets you keep an installation in place while adding the interfaces and automation of Calaos to it.

Calaos does not talk to the bus directly: it goes through **knxd**, a service that bridges the IP network and the KNX bus.

## The knxd service

`knxd` must be reachable from the Calaos server. It most often runs on the Calaos machine itself, in which case the IOs use the default address `127.0.0.1`.

It needs a physical interface to the bus, which can be:

- an **IP / KNX gateway** on the network;
- a **USB interface** plugged into the server;
- a **coupler** built into a PLC, such as the KNX-compatible Wago heads — see [KNX installation]({{% relref "hardware/wago/knx" %}}).

## Group addresses

Everything rests on **group addresses**, written as `x/y/z`. Each function of the bus — switching a lamp on, reading a temperature — has its own, defined when the KNX installation was designed, usually with the ETS software.

{{% notice tip %}}
Get hold of the **ETS project for the installation**, or failing that the list of group addresses. Without it you have to guess what each address does, which is tedious and risky on a live installation.
{{% /notice %}}

## Common parameters

All KNX IOs share the following parameters:

| Name | Type | Required | Description |
|---|---|---|---|
| `knx_group` | string | yes | Group address to control or read, in the form `x/y/z` |
| `listen_knx_group` | string | no | Group address to listen to for the real state |
| `eis` | int | no | KNX data type (EIS), from 0 to 15 |
| `read_at_start` | bool | no | Sends a read request at startup to get the current value. Defaults to `false` |
| `host` | string | yes | Address of knxd. Defaults to `127.0.0.1` |

### The listen address

`listen_knx_group` deserves an explanation, as forgetting it is the most frequent cause of display inconsistencies.

On a KNX bus, the address that **commands** a device and the one that **reports its state** are often different. If you only fill in `knx_group`, Calaos knows what it asked for but not what actually happened: a lamp switched on from a plain KNX button will appear off in the interfaces.

Fill in `listen_knx_group` with the status feedback address, and the display will follow reality whatever the origin of the command.

### The `eis` parameter

KNX encodes its values according to data types. The `eis` parameter tells Calaos how to interpret what travels on the address: a boolean, a percentage, a temperature.

If reported values are nonsensical — a temperature of 6000 degrees, a dimmer jumping from 0 to 100 — it is almost always this parameter that does not match what the device sends.

## Shutters

Shutters are the exception: they use two distinct addresses rather than one.

| Name | Type | Required | Description |
|---|---|---|---|
| `knx_group_up` | string | yes | Group address for opening |
| `knx_group_down` | string | yes | Group address for closing |

## Supported IO types

**As inputs** — switch, long-press switch, triple switch, analog input, temperature sensor.

**As outputs** — light, dimmable light, RGB light, shutter, smart shutter, analog output.

## Creating a KNX IO

In Calaos Installer, **Add → KNX**, then the type you want. Fill in the group address, and the listen address if there is one.

The details of each parameter are displayed directly in Calaos Installer — see [Create IOs]({{% relref "calaos_installer/io" %}}).

## Diagnosis

If an IO does not react, check in this order:

1. **is knxd reachable** from the server, at the address given in `host`?
2. **does the group address** really match the function you are targeting in the ETS project?
3. **does the `eis` data type** match what the device expects?

Exchanges with the bus appear in the server logs, under the `knx` domain:

```sh
calaos_config set debug_enabled true
calaos_config set debug_domains knx:5
systemctl restart calaos-server
journalctl -u calaos-server -f
```

Remember to set `debug_enabled` back to `false` afterwards — see [Logs]({{% relref "calaos_os/configuration/logs" %}}).
