+++
title = 'Input module'
date = 2024-03-06T20:02:53Z
weight = 30
+++

## 4, 8, 16 Channel Input Modules

Paired with the fieldbus controller, the 4, 8, 16 channel 24V DC digital input modules allow for the connection of push buttons as well as any on/off signal inputs.

## Wago Modules

The terminal block allows for the connection of 4, 8, or 16 dry contacts and is galvanically isolated.

- 750-402: 4 inputs 24V
- 750-430: 8 inpuots 24V
- 750-1405: 16 inputs 24V

![module](/en/hardware/wago/images/borne_entree8.jpg?width=10pc&classes=shadow)
![module](/en/hardware/wago/images/borne_entree16.jpg?width=10pc&classes=shadow)

## Wiring

The wiring of switches is done with 24V low voltage by injecting +24V into the input terminal. This can be done as shown in the figure below, using the PLC's 24V power supply.

![module](/en/hardware/wago/images/schema_inter.png?width=20pc&classes=shadow)

## Programming

Configuring a switch is done using the [Calaos Installer]({{%relref "calaos_installer/wago" %}}) software. You may need to know the input number of the switch on the PLC. For example, in the diagram above, switch 1 is connected to input 0 of the PLC, and switch 2 to input 1 of the PLC.

It's important to note that the numbering of input terminals starts from 0 for the first terminal and then continues with the following input terminals. Thus, the first terminal is numbered 0, the next one 8 (for 8-channel terminals), and so on.
