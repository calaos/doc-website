+++
title = 'Zigbee'
date = 2024-03-06T20:04:35Z
weight = 40
summary = 'Zigbee support with zigbee2mqtt'
+++

## Zigbee with Zigbee2MQTT on Calaos

### Introduction

**Zigbee** is a wireless communication protocol designed for home and industrial automation applications, offering low power consumption and extended range thanks to its mesh network. It allows various devices such as connected bulbs, sensors, and switches to communicate with each other.

**Zigbee2MQTT** is a gateway that allows controlling Zigbee devices via MQTT without the need for a proprietary gateway. It translates Zigbee messages into MQTT messages, facilitating integration with home automation systems like Calaos.

#### How Zigbee2MQTT Works

- **Zigbee Adapter**: A USB dongle or an Ethernet module that connects to the Zigbee network.
- **Zigbee2MQTT Server**: Software running on your Calaos server that manages communication between Zigbee devices and MQTT.
- **MQTT Broker**: Messaging server that relays messages between Zigbee2MQTT and Calaos. It must be activated beforehand [MQTT]({{% relref "hardware/mqtt" %}})

#### Role of Zigbee Routers

Zigbee devices can act as **routers** or **coordinators** to extend the network range. By adding mains-powered devices (such as smart plugs or connected bulbs), you can increase the Zigbee signal coverage throughout your home.

---

### Activation and Configuration of Zigbee2MQTT on Calaos

#### Prerequisites

- A compatible Zigbee adapter:

  - **Either a USB adapter (like the ConBee II)**. The USB adapter must be connected to your Calaos server. If it is located in a basement, communication may be difficult with devices on upper floors.
  - **Or a Network adapter (like the SMLIGHT SLZB-06)**. Can be placed anywhere in the house if there is a network outlet. Moreover, the SLZB-06 is POE (Power over Ethernet)
  
- Consult the complete list of supported adapters: [Zigbee2MQTT Adapters](https://www.zigbee2mqtt.io/guide/adapters/)

> The type of adapter will determine how to configure zigbee2mqtt.

#### Activation Steps

1. **Configuration of Zigbee2MQTT**

   The configuration file is located at:

   ```bash
   /mnt/calaos/zigbee2mqtt/data/configuration.yaml
   ```

   > You can edit this file using the `nano` command-line editor.

   The configuration looks like this:

   ```yaml
   # Home Assistant integration (MQTT discovery)
   homeassistant: false

   # Allow new devices to join
   permit_join: true

   # MQTT settings
   mqtt:
     # MQTT base topic for zigbee2mqtt MQTT messages
     base_topic: zigbee2mqtt
     # MQTT server URL
     server: 'mqtt://127.0.0.1'
     # MQTT server authentication, uncomment if required:
     # user: my_user
     # password: my_password

   # Serial settings
   serial:
     # Location of CC2531 USB sniffer
     port: /dev/ttyACM0

   frontend: true
   ```

   Depending on the chosen adapter, you will need to configure the `port` option correctly to allow Zigbee2MQTT to use it:

   - **For the ConBee II (USB)**:

     ```yaml
     serial:
       port: '/dev/ttyACM0'
     ```

   - **For the SMLIGHT SLZB-06 (Ethernet POE)**:

     ```yaml
     serial:
       port: 'tcp://slzb-06.local:6638'
     ```

   You will also need to modify the MQTT parameters if necessary, particularly `username` and `password`.

   > By default in Calaos-OS, MQTT is configured without authentication. If you want more security, it is preferable to set up a username and password.

2. **Activation of Zigbee2MQTT**

   Zigbee2MQTT is already included in **Calaos OS**. To activate it, execute the following command in your Calaos server terminal:

   ```bash
   sudo systemctl enable zigbee2mqtt.service
   sudo systemctl restart zigbee2mqtt.service
   ```

3. **Restarting the Service**

   After modifying the configuration, restart the service to apply the changes:

   ```bash
   sudo systemctl restart zigbee2mqtt.service
   ```

---

### Using Zigbee2MQTT via the Web Page

#### Accessing the Web Interface

- Open a web browser.
- Go to `http://<calaos-ip-address>:8080`.
- You will see the Zigbee2MQTT dashboard with the list of connected devices.

   On this interface, you will need to pair and configure the Zigbee devices before adding them to Calaos.

---

### Pairing a Device and Configuration in Zigbee2MQTT

#### Pairing the Device

1. **Activate Pairing Mode**

   On the Zigbee2MQTT web interface, click on the **"Permit join (All devices)"** button to allow pairing of new devices.

2. **Put the Device in Pairing Mode**

   Follow the manufacturer's instructions to put your device in pairing mode. You can also consult the device's page on the Zigbee2MQTT website to learn how to perform the pairing.

   > Example: For a TRADFRI connected bulb, the explanation can be found here: [Zigbee2mqtt TRADFRI](https://www.zigbee2mqtt.io/devices/LED1546G12.html)

3. **Verification of Pairing**

   Once the device is detected, it will appear in the list with a default identifier.

### Configuring the Device

1. **Rename the Device**

   - Click on the device in the list.
   - Assign it a friendly name for easy identification in Calaos.

2. **Add Routers to Extend the Network**

   - Install mains-powered devices such as connected plugs or bulbs.
   - These devices will act as Zigbee routers, extending the network's range.

---

### Adding the Device in Calaos with Calaos Installer

Calaos uses MQTT to communicate with Zigbee2MQTT. Therefore, you will need to add an MQTT IO for each device you wish to control in Calaos. Keep in mind that an IO in Calaos represents a **value**. Let's take the example of a temperature and humidity sensor [Aqara](https://www.zigbee2mqtt.io/devices/WSDCGQ12LM.html). We see in the documentation that this device will **expose** several values: `temperature`, `humidity`, `pressure`, `battery`, etc. In Calaos, you will need to create an IO for each property: one IO for temperature, one IO for humidity, etc.

In Calaos Installer, there is a dedicated tool that can connect to the MQTT Broker and detect available Zigbee devices on Zigbee2MQTT. You will need to select the property with which you wish to create an IO, and the assistant will configure it for you.

![zigbee2mqtt](/en/hardware/images/z2q_01.png?width=10pc&classes=shadow)

![zigbee2mqtt discovery](/en/hardware/images/z2q_02.png?width=20pc&classes=shadow)

---

## Additional Resources

- **Zigbee2MQTT Documentation**: [zigbee2mqtt.io](https://www.zigbee2mqtt.io/)
- **Compatible Adapters**: [Zigbee2MQTT Adapters](https://www.zigbee2mqtt.io/guide/adapters/)
- **Supported Zigbee Devices**: [Zigbee2MQTT Devices](https://www.zigbee2mqtt.io/supported-devices/)
- **MQTT Documentation in Calaos**: [Calaos MQTT]({{% relref "hardware/mqtt" %}})
