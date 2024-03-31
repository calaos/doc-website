+++
title = 'Analog'
date = 2024-03-28T14:30:48Z
+++

## Analog Input or Output Module

Among the data that one may want to measure in a house, there are many sensors that do not provide binary outputs. These sensors are connected in various ways and return an analog value. For instance, humidity sensors, sunlight sensors, brightness sensors, etc., fall into this category. Similarly, certain equipment can be controlled analogically in the range of 0-10V, such as ventilation regulators.

The Wago system allows connecting these types of sensors through analog terminals. There are different types of terminals depending on the type of data returned by the sensor or the type of equipment to be controlled. Therefore, we have terminals for 0-10V, 4-20mA, -10V/+10V, etc. It is best to consult Wago's website to find the appropriate terminal.

![module](/en/hardware/wago/images/borne_analog_0-10v.jpg?width=10pc&classes=shadow)
![module](/en/hardware/wago/images/borne_outanalog_0-10v.jpg?width=10pc&classes=shadow)

## Wiring

The wiring depends on the type of equipment and terminal. However, it remains relatively simple and is similar to the wiring of temperature sensors. It is advisable to consult Wago's documentation and that of the equipment to be controlled.

## Programming

Configuring an output is done using the [Calaos Installer]({{%relref "calaos_installer/wago" %}}) software. You will need to use a Wago analog input or output in Calaos Installer, then specify the variable number to be controlled.
