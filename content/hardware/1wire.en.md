+++
title = '1-Wire'
date = 2024-03-06T20:04:56Z
weight = 60
summary = '1-Wire Temperature Sensors'
+++

## 1-Wire with Temperature Sensors on Calaos

### Introduction

**1-Wire** is a communication protocol developed by Dallas Semiconductor (now Maxim Integrated) that allows communication between a master (such as a microcontroller or computer) and multiple 1-Wire devices using a single data wire, plus a ground connection. The protocol is often used for temperature sensors like the **DS18B20**, which provide accurate and affordable measurements.

The **DS9490R USB gateway** allows connecting a 1-Wire network to a system like **Calaos** for temperature monitoring, using 1-Wire sensors such as **DS18B20**.

![1wire](/en/hardware/images/one_wire1.png?width=30pc&classes=shadow)

---

### Prerequisites

- **DS9490R Adapter**: A USB adapter to connect the 1-Wire network to a Calaos server.
  ![1wire](/en/hardware/images/one_wire6.png?width=15pc&classes=shadow)
- **DS18B20 Sensors**: 1-Wire compatible temperature sensors.
  
  These sensors can be wired in two ways: powered mode (3 wires) or parasite mode (2 wires, power via the data line).

---

### Types of Sensors

- **Wired Model**: The sensor is enclosed in a waterproof stainless steel casing. It has 3 wires:
  - Red wire: Power (5V). Labeled VDD, VCC, V+ in documentation.
  - Yellow wire: 1-Wire data.
  - Black wire: GND, 1-Wire ground (0V).

![1wire](/en/hardware/images/one_wire2.png?width=15pc&classes=shadow)

- **Bare Model**:
  
![1wire](/en/hardware/images/one_wire3.png?width=20pc&classes=shadow)

---

### Wiring

![1wire](/en/hardware/images/one_wire4.png?width=30pc&classes=shadow)

#### Powered Mode

In this mode, the DS18B20 sensor uses a dedicated power supply in addition to the data line and ground:

- **Data (DQ)**: Connected to the 1-Wire data line.
- **Power (Vdd)**: Connected to a 3.3V or 5V power supply.
- **Ground (GND)**: Connected to the common ground.

This mode is recommended because it provides more stable communication, especially over longer distances.

#### Parasite Mode

In parasite mode, the DS18B20 sensor uses the data line for power, eliminating the need for the Vdd connection:

- **Data (DQ)**: Connected to the 1-Wire data line.
- **Ground (GND)**: Connected to the common ground.

This mode is easier to wire but may be less reliable, especially over long distances or with multiple sensors.

#### DS9490R Wiring

![1wire](/en/hardware/images/one_wire7.png?width=30pc&classes=shadow)

---

### Sensor Identification

Once your 1-Wire sensors are properly connected to the DS9490R adapter, you will need to retrieve the unique IDs of the sensors to assign them to specific rooms.

> It is recommended to connect the sensors one by one and retrieve their unique ID once the new sensor is connected. This will help avoid confusion with the IDs in case multiple sensors are connected.

#### Scanning Sensors

In Calaos-OS, the `calaos_1wire` tool is used to scan the 1-Wire bus to list the sensors and their temperatures.

> Note: You need to temporarily stop the `calaos-server` service to scan the bus. Both cannot run at the same time, and `calaos_1wire` will return an error.

In the terminal, use the following command:

```bash
# stop calaos-server
systemctl stop calaos-server

calaos_1wire --scan -u

# restart calaos-server
systemctl start calaos-server
```

> The `-u` option tells the tool to use the USB gateway. If you are using another gateway, you will need to use a different connection option. Refer to the OWFS documentation in that case.

```shell
[INF] extern_process (OWExternProc_main.cpp:281) OW Library initialization ok
Scanning for 1wire devices... found 6 devices.
Device: 28.74DDB4040000 (DS18B20) --> value: 22.875
Device: 28.3C2DB4040000 (DS18B20) --> value: 21.8125
Device: 28.BE3BB4040000 (DS18B20) --> value: 23.625
Device: 28.F53416122101 (DS18B20) --> value: 21.8125
Device: 28.FF31563A0400 (DS18B20) --> value: 19.375
Device: 28.FFED9A3C0400 (DS18B20) --> value: 19.5625
```

This command will return the unique addresses of the connected sensors, in the form of a unique identification number (ID) for each sensor. Note these IDs as they will be necessary for adding them in Calaos Installer.

> Example: `28.74DDB4040000` for the first sensor.

---

### Adding Sensors in Calaos Installer

To add a temperature sensor in Calaos Installer, simply provide the sensor's ID and the connection option to the 1-Wire gateway (in this case `-u` in the example for the **DS9490R** gateway).

![1wire](/en/hardware/images/1w_01.png?width=10pc&classes=shadow)
