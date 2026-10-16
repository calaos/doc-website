+++
title = "Example projects"
date = 2026-10-15T10:00:00Z
weight = 50
summary = "Four complete projects to download, from an electric radiator to a reversible floor on a heat pump: open them, try them, adapt them."
+++

These projects are complete: an `io.xml` (rooms, IOs, devices) and a `rules.xml` (rules). They open in Calaos Installer without errors. Examples 1, 3 and 4 even work **without hardware**: you can try them before adapting them to your house. Example 2 needs its air conditioner.

| Project | What it shows | Download |
|---|---|---|
| **1. One electric radiator** | an internal thermostat on a relay, a weekly schedule, stop on open window ([recipes 1 and 3]({{% relref "calaos_installer/rules_heating" %}})) | [1-radiateur-electrique.zip](/examples/climat/1-radiateur-electrique.zip) |
| **2. An ESPHome air conditioner** | a unit that regulates by itself (`proxy` device), the room sensor relayed to the unit, absence and frost protection ([ESPHome air conditioner]({{% relref "hardware/esphome_climate" %}}), recipe 4) | [2-clim-esphome.zip](/examples/climat/2-clim-esphome.zip) |
| **3. A floor heating, 3 zones** | three floor thermostats, one valve per zone, a common circulator driven by rules (recipe 5) | [3-plancher-chauffant-3-zones.zip](/examples/climat/3-plancher-chauffant-3-zones.zip) |
| **4. A reversible floor, 9 zones** | nine single-valve zones, the house mode, the two heat pump contacts, domestic hot water (recipes 6 and 7) | [4-plancher-reversible-pac.zip](/examples/climat/4-plancher-reversible-pac.zip) |

Each archive holds a folder with `io.xml`, `rules.xml` and a `README.txt` listing what to replace. The names are in French, as in the screenshots of this documentation ("PAC" is the heat pump, "ECS" the domestic hot water, "Plancher" the floor).

## Open an example

1. Unzip the archive: you get a folder.
2. In Calaos Installer, **File** menu, **Open a projet...**, and choose that folder.

The project opens without an error message. Its devices are edited like yours: see [Create a device]({{% relref "calaos_installer/devices/templates" %}}).

## IOs to replace

The IOs whose name ends with **"(à remplacer)"** (to be replaced) are [internal variables]({{% relref "calaos_installer/internal_var" %}}): a sensor is a numeric variable, a relay or a valve a boolean one. They stand in for your hardware, so that the project works as it is.

To try it, send the project to a test box, then change the temperature of a sensor from an interface: the thermostat output follows. A placeholder sensor is only updated when you change it. After `sensor_timeout` (2.5 h) without a change, the thermostat considers it silent and switches its output off: that is the normal safety.

To put your hardware in its place, two ways:

- **in Calaos Installer**: create the real IO (sensor, relay), open the device and choose it for the link concerned (see [Create a device]({{% relref "calaos_installer/devices/templates" %}})). Fix the rules that used the old IO, then delete the placeholder IO;
- **in `io.xml`**, with a text editor: replace the line of the IO with the one of your hardware, **keeping the same `id`**. The device and the rules follow with no other change.

Example 2 is different: its IOs are the real MQTT IOs of an ESPHome unit, but they target the broker `127.0.0.1` and the node `clim_salon`. The simplest is to create the device again with the **ESPHome climate** template of the wizard and its broker search, keeping the same name, then to check the rules.

## Send the project to a box

**Calaos Server** menu, **Send project to Calaos Server...**: see [Upload config]({{% relref "getting_started/upload" %}}).

{{% notice warning %}}
The project **replaces** the whole configuration of the box. If you want to keep yours, fetch it first with **Download project from Calaos Server...**, or try the example on a test box.
{{% /notice %}}
