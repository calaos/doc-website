+++
title = 'Create IOs'
date = 2024-03-06T20:12:25Z
weight = 20
summary = "Declaring your devices: lights, shutters, switches, sensors."
+++

## What an IO is

An **IO** — for input/output — represents an element of your home: a lamp, a shutter, a switch, a temperature sensor, a switched socket.

Calaos distinguishes two families, and this distinction shapes everything else:

| Family | Role | Examples |
|---|---|---|
| **Input** | Calaos **reads** a piece of information | switch, sensor, detector, meter |
| **Output** | Calaos **acts** on something | lamp, shutter, socket, heating |

A lamp that comes on when a switch is pressed is therefore **two IOs** — an input and an output — tied together by a rule. They are not linked automatically: you decide the link, which then lets any switch control any lamp.

## Adding an IO

Select the room concerned, then open the **Add** menu. You first choose **the hardware family**, then **the device type**.

![Light menu](/en/getting_started/images/menu_light.png?width=20pc&classes=shadow)

For example, for a lamp driven by a Wago PLC: **Add → Wago PLC → Light**. All that remains is to give it a name and the address of the output on the PLC.

![New light](/en/getting_started/images/wago_light.png?width=20pc&classes=shadow)

The IO then appears in the selected room.

![IO list](/en/getting_started/images/io.png?width=20pc&classes=shadow)

## The hardware families

Each family corresponds to a way of talking to your devices, and has its own page in the [Supported hardware]({{% relref "hardware" %}}) chapter:

| Family | Documentation |
|---|---|
| Wago PLC | [Wago PLC]({{% relref "hardware/wago" %}}) |
| KNX | [KNX]({{% relref "hardware/knx" %}}) |
| MQTT | [MQTT]({{% relref "hardware/mqtt" %}}) |
| Zigbee | [Zigbee]({{% relref "hardware/zigbee" %}}) |
| 1-Wire | [1-Wire]({{% relref "hardware/1wire" %}}) |
| DMX / OLA | [DMX]({{% relref "hardware/dmx_ola" %}}) |
| Philips Hue | [Hue]({{% relref "hardware/hue2mqtt" %}}) |
| GPIO | [GPIO]({{% relref "hardware/gpio" %}}) |
| WebIO | [WebIO]({{% relref "hardware/webio" %}}) |
| Cameras | [Cameras]({{% relref "hardware/camera" %}}) |
| Remote UI screens | [Remote UI]({{% relref "hardware/remote_ui" %}}) |

Some IOs depend on no hardware at all: [internal variables]({{% relref "calaos_installer/internal_var" %}}), [scenarios]({{% relref "calaos_installer/scenario" %}}) and [time scheduling]({{% relref "calaos_installer/time" %}}) objects are handled exactly like any other IO in rules.

## The device types

Whatever the family, the same broad types come back:

**As inputs** — simple switch, long-press switch, triple switch, analog input, temperature sensor, numeric value, character string.

**As outputs** — simple light, dimmable light, RGB light, shutter, smart shutter, analog output, character string.

{{% notice tip %}}
**Long press is a switch type in its own right.** Rather than hacking a rule together with a timer, choose the right type at creation time: Calaos then tells a short press from a long one, and you can assign a different action to each.
{{% /notice %}}

The type also determines **how the device is displayed** in the interfaces: a shutter gets up/down buttons, a dimmable light gets a slider.

## The properties of an IO

Once the IO is created, its properties window gives access to settings common to every type:

| Property | Effect |
|---|---|
| **Name** | The label displayed everywhere |
| **Visible** | Shows or hides the IO in the interfaces |
| **History** | Records state changes in the event log |
| **Value logging** | Sends measurements to the history database, to plot graphs |

Hiding an IO does not disable it: it keeps working in rules, it simply does not clutter the interfaces. This is useful for technical inputs nobody needs day to day.

{{% notice note %}}
Each IO type additionally has its own parameters — address, port, device identifier. **Calaos Installer displays built-in help for each of them**: it is the most up-to-date description, since it comes straight from the server.
{{% /notice %}}

## Naming your IOs

As with rooms, these names will be read every day.

The name does not need to repeat the room: interfaces already display the IO **inside** its room. "Kitchen — Kitchen light" is redundant; "Ceiling light" is enough.

Do however clearly distinguish similar devices in the same room: "Ceiling light" and "Worktop spots" rather than "Light 1" and "Light 2".

## What comes next

With your devices declared, it remains to make them react: [Create rules]({{% relref "calaos_installer/rules" %}}).
