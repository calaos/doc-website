+++
title = 'DMX Dimming'
date = 2024-03-06T20:03:14Z
weight = 60
+++

## DMX Interface Module

### DMX 512

Among the lighting control methods, there are several well-defined standards that have been widely used for a long time. One of these standards is [DMX512](https://en.wikipedia.org/wiki/DMX512), commonly known as DMX (Digital Multiplexing).

It is primarily used in the stage industry (concerts, TV sets, light and sound shows) for dynamic lighting control. This protocol has some advantages for residential home automation use. DMX512 is currently the most widespread and universal protocol, used everywhere and by all manufacturers of stage lighting equipment.

Therefore, power dimmer blocks capable of controlling multiple devices are available at very affordable prices. These blocks can also support higher power loads than what could be achieved with DALI.

## Implementation

To facilitate integration with a Calaos system, we have chosen to use a simple DMX ↔ Ethernet gateway. This gateway converts commands from the Ethernet network and transmits them over the DMX link.

The gateway used is available at [DMX4ALL](https://shop.dmx4all.de/de/dmx4all-produkte/dmx-interfaces/usb-dmx-interfaces/) under the reference LAN-DMX-STAGE-PROFI.

![module](/en/hardware/wago/images/lan_dmx_stage_profi.jpg?width=10pc&classes=shadow)

## Wiring

This gateway should be connected to the automation network, either through a switch or through the double network port of the 750-849 controller head. The DMX blocks will be connected to the DMX bus. It is advisable to place a 120Ω termination resistor at the end of the DMX bus on the last DMX device.

![module](/en/hardware/wago/images/schema_dmx.png?width=20pc&classes=shadow)

## Programming

### Addresses

Since the DMX gateway communicates with the controller over the Ethernet network, it should have the following default IP address: **10.0.0.120**. It is possible to change this IP using Calaos Installer on the controller.

![module](/en/hardware/wago/images/calaos_wago_dmx.jpg?width=10pc&classes=shadow)

For simplicity in managing the Calaos system, a DMX device is treated as a DALI device. This allows the controller to handle DALI and DMX lighting in the same way. Programming becomes entirely transparent. However, it should be noted that DMX address 1 is equivalent to DALI address 101, DMX 2 corresponds to DALI 102, and so on.

Programming is also done using the [Calaos Installer]({{%relref "calaos_installer/wago" %}}) software. You can adjust the lighting intensity just like with DALI lighting elements.

### Programming

Programming is also done using the [Calaos Installer]({{%relref "calaos_installer/wago" %}}) software. However, you can adjust the lighting intensity unlike with conventional lighting elements.
