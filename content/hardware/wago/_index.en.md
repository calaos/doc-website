+++
title = 'Wago PLC'
date = 2024-03-06T20:02:33Z
weight = 10
summary = 'Wago PLC of the type 750-841, 750-880, ...'
+++

## Wago PLC

![module](/en/hardware/wago/images/20080819-IMG_9203.jpg?width=40pc&classes=shadow)

The Wago PLC forms the foundation of a Calaos installation. Equipped with industrial components to ensure quality and durability, this PLC is responsible for controlling all the electrical elements in the house, as well as acquiring the state of switches, temperature probes, sensors, and much more. It thus plays a crucial role in bridging the computer world of the central unit and the electrical world.

### Safety

The PLC also incorporates a backup program in case the main computer system fails. In the event of a communication breakdown with Calaos, it switches to autonomous mode. This ensures increased system reliability, allowing users to interact with their home as with a traditional electrical installation, thereby mitigating concerns related to computer failures.

### Range of PLCs

The advantage of the Wago PLC lies in its modularity. It is therefore scalable and adaptable to all types of installations. For example, it is always possible to add a module later to control an additional room. The main module incorporates the system's intelligence and a network communication port to interact with the computer system and receive commands from the Calaos server. Depending on the version, this module may also include an additional Ethernet port for the KNX or DMX protocol. The PLC allows connecting to its internal bus analog and digital input/output terminals, as well as terminals specific to other communication buses (DALI, KNX, etc.).

We mainly use these PLC heads:

- The 750-841 head (Replaced by 750-881, 750-880, 750-891, 750-893)
- The 750-849 head for installations requiring KNX compatibility. (Replaced by 750-889)

For more information on a Calaos installation compatible with KNX, you can visit this page: [KNX Installation]({{%relref "hardware/wago/knx" %}})

## Electrical Safety

From a safety standpoint, the Wago PLC complies with the IP20 protection standard by incorporating galvanic isolation between the internal electronics and the field. This protection against short circuits is ensured by optocouplers, among other components.

## Modules and Extensions

To collect information from switches, we use digital input terminals. These terminals operate with low currents of 24V DC and function on the principle of [on/off](https://en.wikipedia.org/wiki/Bang%E2%80%93bang_control). The output modules, on the other hand, allow controlling elements such as lighting or motorized blinds.

Here is a list of different modules and extensions that can be added to a Wago PLC.

### Switches, binary sensors

- [4,8,16 channel 24V input module]({{%relref "hardware/wago/input" %}})

### Lighting points, blinds, shutters, on/off outputs

- [4,8,16 channel 24V output module]({{%relref "hardware/wago/output" %}})

### Light dimming

- [DALI interface module]({{%relref "hardware/wago/dali" %}})
- [DMX module]({{%relref "hardware/wago/dmx" %}})

### Heating, various, ...

- [PT100/PT1000 temperature probe input module]({{%relref "hardware/wago/pt1000" %}})
- [Analog input/output module]({{%relref "hardware/wago/analog" %}})

### Bus extender

- [Extending a Wago bus]({{%relref "hardware/wago/750-627" %}})
