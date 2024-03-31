+++
title = 'Output module'
date = 2024-03-06T20:02:58Z
weight = 40
+++

## 4, 8, 16 Channel Output Modules

To control electrical elements such as lighting fixtures, electric blinds/shutters, controlled sockets, etc., we use a 24V DC digital output module. This on/off output terminal allows connecting up to 8 actuators. Typically, these actuators are 24V/220V 16A power relays, capable of supporting the load of lighting fixtures and controlled sockets in a house.

The terminal allows connecting 8 dry contacts and is galvanically isolated.

![module](/en/hardware/wago/images/borne_sortie8.jpg?width=10pc&classes=shadow)
![module](/en/hardware/wago/images/borne_sortie16.jpg?width=10pc&classes=shadow)

## Wiring

The wiring of outputs to relays is done at low voltage as shown in the diagram below.

![module](/en/hardware/wago/images/schema_relais.png?width=20pc&classes=shadow)

### Lighting Points, Controlled Socket

The diagram shows the connection of 2 lighting points. The principle is exactly the same for a controlled socket. Since the power relays have a breaking capacity of 16A, they are sufficient for use in these cases.

### Shutter

The case of the shutter is also the same. Indeed, a shutter **must be wired with up and down controls**. It then connects like 2 lighting points, using 2 relay outputs. The programming will then use one of the outputs for up and the other for down.

Two operating modes of shutters are compatible. The classic mode, where the relays must be held to perform an up/down action, and the pulse control mode, where a pulse is sufficient to give the order to the shutter. The programming will allow choosing the appropriate mode and the central unit will do the necessary work.

### Heating

The case of heating is similar in certain cases where a heating zone can be controlled with an on/off output. Usually, a heating zone can easily be controlled by connecting a solenoid valve to the desired circuit, then connecting this solenoid valve to a relay output.

## Programming

Configuring an output is done using the [Calaos Installer]({{%relref "calaos_installer/wago" %}}) software. You will need to know the output number of the PLC. For example, in the diagram above, lamp 1 is connected to output 0 of the PLC and lamp 2 to output 1 of the PLC. Therefore, it is necessary to assign an identifier to know the number of each light, shutter, etc.

It is important to note that the numbering of the output terminals starts from 0 for the first terminal and then continues with the following output terminals. Thus, the first terminal is numbered 0, the next one 8, and so on.
