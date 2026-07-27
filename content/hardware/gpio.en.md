+++
title = 'GPIO'
date = 2024-03-06T20:05:13Z
weight = 110
summary = 'GPIO using Linux kernel support'
+++

## Introduction

**GPIOs** are the input/output pins found on some boards — Raspberry Pi, industrial boards, mini PCs with an expansion header. Calaos can read and drive them directly, with no gateway and no bus.

It is the simplest way to connect a few devices when you have no PLC: a door contact, a relay, a button.

{{% notice info %}}
GPIOs suit **a handful of points**. For a complete installation the Wago PLC remains preferable: it brings the number of inputs/outputs, the electrical protection and the [failsafe mode]({{% relref "hardware/wago/failsafe" %}}) that an expansion header cannot offer — see [Requirements]({{% relref "hardware/requirements" %}}).
{{% /notice %}}

## Identifying a pin

Each pin carries a **number**, which is the one used by the Linux kernel — not necessarily the one printed on the header nor the physical pinout number.

{{% notice warning %}}
Numbering is the main source of mistakes. On a single board three different numberings often coexist: the physical pin number, the controller number and the kernel one. It is **the last one** that Calaos expects.

Refer to your board's documentation, and test on a known pin before wiring everything.
{{% /notice %}}

## Inverted level

Many setups are **active low**: the contact is closed when the pin is at zero, not at one. This is notably the case for common relay boards, where a relay engages when the output is set to zero.

Without adjustment, everything works backwards: your lamp comes on when it should go off. The `active_low` parameter fixes this without changing any wiring.

## Supported IO types

**As inputs** — switch, long-press switch, triple switch.

**As outputs** — light, shutter, smart shutter.

## Parameters

### Simple inputs and outputs

| Name | Type | Required | Description |
|---|---|---|---|
| `gpio` | int | yes | Pin number, from 0 to 65535 |
| `active_low` | bool | no | Enable if the level is inverted. Defaults to `false` |

### Shutters

A shutter uses two pins, one per direction.

| Name | Type | Required | Description |
|---|---|---|---|
| `gpio_up` | int | yes | Pin number for opening |
| `gpio_down` | int | yes | Pin number for closing |
| `active_low_up` | bool | no | Inverted level for opening. Defaults to `false` |
| `active_low_down` | bool | no | Inverted level for closing. Defaults to `false` |

## Creating a GPIO IO

In Calaos Installer, **Add → GPIO**, then the type you want. Fill in the pin number, and tick the inverted level if needed.

## Electrical precautions

{{% notice warning %}}
**GPIOs work at very low voltage and can drive no load.** You never connect a 230 V circuit directly to them: you must go through a relay or a suitable control module.

A wiring mistake on an expansion header usually destroys the board, and sometimes more.
{{% /notice %}}

For inputs, check the voltage your board expects: applying 5 V to an input designed for 3.3 V will damage it.

## Diagnosis

If an IO does not react:

1. **is the pin number** the kernel one, not the header one?
2. **is the behaviour simply inverted?** In that case `active_low` is the answer.
3. **is the pin already used** by another function of the board — serial port, I2C bus — in which case it is not available?

The server logs report pin access errors: see [Logs]({{% relref "calaos_os/configuration/logs" %}}).
