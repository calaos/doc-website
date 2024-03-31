+++
title = 'DALI Dimming'
date = 2024-03-06T20:03:09Z
weight = 50
+++

## DALI Interface Module

One of the main advantages of home automation is lighting management. To properly implement it, the ability to adjust the intensity of light points is essential. A light dimming technology uses the [DALI (Digital Addressable Lighting Interface)](https://en.wikipedia.org/wiki/Digital_Addressable_Lighting_Interface) standard.

![module](/en/hardware/wago/images/borne_dali.jpg?width=20pc&classes=shadow)

The coalition of suppliers around a standard has allowed for interoperability in controlling electronic ballasts. This new standard is gradually replacing the old 0-10V dimming interface. The Wago 750-641 is a DALI master module, capable of controlling up to 64 slave ballasts. The ballasts come in different types such as:

- Transformers for spotlights
- Transformers for LED lighting
- RGB tricolor transformers for LED lighting
- Electronic ballasts for compact fluorescent tubes

Two DALI terminals are available: the older generation DALI terminal, **750-641**, and the DALI Master terminal, **750-647**. It's important to note that these terminals offer different functionalities in terms of configuration in Calaos.

The older generation DALI terminal, model **750-641**, is designed to operate only as a DALI slave. It's suitable for traditional applications where simple integration with other masters is not necessary.

On the other hand, the DALI Master terminal, model **750-647**, offers advanced functionalities as a DALI master. It supports not only the control of multiple DALI devices as a master but also operates as a multi-master master, allowing support for DALI motion or brightness sensors (such as the Tridonic M-Sensor2, for example).

## Installation of the Terminal in the PLC

The DALI terminal must be installed directly after the digital I/O modules. It's imperative to install any other analog terminal **after the DALI terminal**. The calaos_wago program supports only one DALI terminal. Therefore, it's not possible to have multiple DALI terminals (master or not).

## Wiring

![module](/en/hardware/wago/images/wago-288-895.jpg?width=5pc&classes=shadow)

The wiring of DALI elements differs entirely from traditional lighting points. This time a 2-wire communication bus needs to be set up. Moreover, this bus requires a specific dedicated 18V power supply.

As mentioned above, DALI is an interface for controlling lighting electronic ballasts and transformers. Thus, the desired lighting fixtures must be equipped with DALI-compatible transformers.

![module](/en/hardware/wago/images/schema_dali.png?width=20pc&classes=shadow)

Each ballast can be connected to the bus. The DALI bus does not require a specific polarity for connection, but care must be taken not to create loops or short circuits. A maximum of 64 slaves can be connected to the terminal. Note that some RGB transformers use 3 addresses (one for each color).

It's recommended to use a minimum of 1.5mm² cable for the 2-wire DALI bus. A smaller section could cause signal interference.

## Programming

### Addresses

Once the ballasts are connected to the bus, they need to be identified.

#### 750-641

You will need to use the [Calaos Installer]({{%relref "calaos_installer/wago" %}}) software and perform an address search. The principle of the DALI bus is simple: each element is assigned an address ranging from 1 to 64. The control unit and the PLC need to know the address at which the equipment can be controlled.

- Disconnect the Calaos server
- Open Calaos Installer, connect to the PLC, and use the **DALI Configuration...** tool

![module](/en/hardware/wago/images/calaos_installer_dali.png?width=20pc&classes=shadow)

It is possible to do the addressing and configure the DALI groups directly in the interface.

#### 750-647

The addressing of DALI equipment should be done with the **Wago Dali Configurator** software. This software only works with the 753-647 terminal. On the DALI bus with the master terminal, 64 ballast addresses are supported. In addition, there are 14 addresses for detectors. However, a detector like the Tridonic M-Sensor2 requires 3 addresses. Be careful not to exceed these limits (which are related to the DALI bus).

##### Wago Dali Configurator

- Disconnect the Calaos server
- Set the Wago to STOP mode via the switch (central position)
- Launch Wago Dali Configurator
- Start a search for unaddressed devices
- To find the color corresponding to the address, you can light up the selection
- Configure the ballasts and write the configuration (In the example, there is an RGBW ballast (⇒ 4 addresses))

{{% notice note %}}
It is recommended to change the ballasts' address to a number higher than 3. Indeed, if a ballast is added later, it might conflict with the existing ballasts.
{{% /notice %}}

![module](/en/hardware/wago/images/daliconfiguratorparametres.jpg?width=20pc&classes=shadow)

{{% notice note %}}
The terminal position doesn't correlate with the card's position on the bus but rather with the number of DALI cards (1).
{{% /notice %}}

![module](/en/hardware/wago/images/daliconfiguratoradressage.jpg?width=20pc&classes=shadow)

![module](/en/hardware/wago/images/daliconfiguratorpoweron.jpg?width=20pc&classes=shadow)

![module](/en/hardware/wago/images/daliconfiguratorwriteparam.jpg?width=20pc&classes=shadow)

##### Configuration in Calaos Installer

Once in Calaos Installer, simply adding Wago Dali devices isn't enough for it to work with the 750-647 terminal. It's also necessary to declare all devices on the bus and send this configuration to the PLC.

To do this, you need to connect to the PLC with Calaos Installer. Then go to the **Wago PLC** → **Upload Dali Master 647 Configuration** page.

![pic](/en/hardware/wago/images/calaos_installer_dali_master.jpg?width=20pc&classes=shadow)

In this tool dedicated to the Dali Master terminal, list all Dali devices on the bus along with their respective addresses. The **Load from PLC** button allows reloading a configuration stored in the PLC. The **Send to PLC** button does the opposite, sending the configuration to the PLC. The PLC will automatically restart.

After this configuration is complete, Calaos Installer can be used in the traditional manner by adding Dali Light objects.

##### Addresses

##### Ballasts

The addresses for DALI ballasts start at 1 in Calaos. This is not the case in the Wago Dali Configurator, so care should be taken.

##### Sensors

To use presence detectors or Dali switches, you must use classic Wago Switches in Calaos Installer. Their addresses start at **6656**.

Ex:

| DALI Address | Wago Dali Configurator Address | Calaos installer Switch Address |
|----------|------|------|
| 1 | 0 | 6656 |
| 2 | 1 | 6657 |
| 3 | 2 | 6658 |
| 4 | 3 | 6659 |
| ... | ... | ... |

When setting up detectors in Calaos Installer, you can specify an _Off Timeout_. This refers to the duration it takes for the detector to switch back to the low state after activation.

##### Luxmeter

It's possible to retrieve the value of luxmeters in Calaos. To do this, create a Wago Input Analog IO in Calaos Installer and assign it the DALI address + 255.

Ex:

| DALI Address | Wago Dali Configurator Address | Calaos installer Sensor Address |
|----------|------|------|
| 1 | 0 | 256 |
| 2 | 1 | 257 |
| 3 | 2 | 258 |
| 4 | 3 | 259 |
| ... | ... | ... |

#### Example with 750-647

For the following detector:

{{% notice note %}}
Multi Sensor Type 2: Presence address: 1 Light address: 2 IR address: 3 Offtimeout: 10000ms
{{% /notice %}}

In Calaos Installer, the following IOs can be declared:

- Wago Switch address: 6656
- Wago Switch address: 6658
- Wago Analog Input address: 257

For the detector, the following rule can be created:

    IF SensorPresence == True
    IF SensorLux <= 50
    Then LightDali = impulse 600000

If the presence detector is triggered and the luxmeter reads below 50 Lux, then turn on the DALI light for 10 minutes.

### Programming

Programming is also carried out using the Calaos Installer software. Unlike classic lighting elements, you can play with the intensity of the lights.
