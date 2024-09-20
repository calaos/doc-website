+++
title = 'DMX using OLA'
date = 2024-03-06T20:04:46Z
weight = 50
summary = 'DMX Lights dimming using Open Light Architecture'
+++

## OLA (Open Lighting Architecture) on Calaos

### Introduction

**DMX** (Digital Multiplex) is a communication protocol primarily used for controlling stage lighting, lighting effects, and other automation equipment. It allows control of many lighting devices (spotlights, dimmers, etc.) via a single cable, sending light instructions to each connected device.

**OLA (Open Lighting Architecture)** is an open-source platform that allows you to manage and control DMX equipment via various types of interfaces, including USB DMX adapters. OLA supports many lighting protocols, such as DMX512, and offers a user-friendly web interface for managing DMX universes and connected devices.

---

### Prerequisites

- **USB DMX Adapter**: You will need a compatible DMX adapter to connect the DMX protocol to your Calaos server. Here are some examples of supported adapters:
  - **Enttec USB DMX Pro**
  - **DMXKing UltraDMX Micro**

For a complete list of supported DMX interfaces, consult OLA's official documentation.

---

### Activation and Configuration of OLA on Calaos

#### Activating OLA

OLA is already included in **Calaos OS**. To activate it, run the following command in your Calaos server terminal:

```bash
sudo systemctl enable olad.service
sudo systemctl start olad.service
```

---

### Configuring OLA via the Web Interface

1. **Access the OLA Web Interface**

   - Open a web browser and navigate to `http://<calaos-ip-address>:9090`.
   - The OLA web interface allows you to manage DMX universes and configure connected interfaces.

2. **Configure the Universe**

   - In the **"Home"** section, click on **"Add Universe"**.
   - Assign a universe number (usually 1 for the first universe) and a name.
   - Ensure that the USB adapter is assigned to this universe to allow DMX control through OLA. To do this, check the selected adapter in the list. If it is not in the list, it means it was not detected by OLA.

3. **Testing the DMX Device**

   - On the homepage, you can go to the created universe and click on the **"DMX Console"** tab.
   - You can use this console to test your DMX channels by sending DMX values to various channels to verify that your DMX equipment is functioning correctly.

---

### Adding the DMX Device to Calaos Installer

1. **Add an OLA DMX IO**

   It is possible to create a simple or RGB dimmer in Calaos Installer. You need to go to the **"Open Light Architecture"** menu for this.

2. **Configure the Input/Output**

   - **Channel**: Select the DMX channel you want to control (e.g., channel 1 for the first lamp, 2 for the second, etc.).
   - **Universe**: Specify the corresponding DMX universe (usually 1 if you have only configured one universe).

---

## Additional Resources

- **OLA Documentation**: [Open Lighting Architecture](https://www.openlighting.org/)
