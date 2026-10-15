+++
title = "Create a device with the wizard"
date = 2026-10-15T09:20:00Z
weight = 15
summary = "The five templates, their default values, the creation wizard and the device editor of Calaos Installer."
+++

In Calaos Installer a device is not built IO by IO: you choose a **template**, fill in a few fields, and the wizard creates the device, all its IOs, links and parameters.

The screenshots show the interface in French.

## The five templates

| Template | Behaviour | What for |
|---|---|---|
| ESPHome air conditioner (`esphome_climate`) | relayed (`proxy`) | ESPHome split or heat pump that regulates by itself; optional relayed room temperature |
| ESPHome infrared air conditioner (`esphome_climate_ir`) | relayed (`proxy`) | air conditioner driven by infrared (`climate_ir`): no action reported, no remote temperature |
| Heating thermostat (`thermostat_heat`) | thermostat | one heating output switched by Calaos |
| Heat / cool thermostat (`thermostat_heat_cool`) | thermostat | two outputs (heat, cool): reversible heat pump on contacts, fan coils |
| Heat / cool floor (`thermostat_floor_heat_cool`) | thermostat | heated and cooled floor, with a cut before the dew point |

## Creating a device

1. In the tree, select the **room** that will receive the device.
2. Open the **Add** menu and choose **Device depuis un template…** (device from a template).

   ![Add menu](/en/climate/images/inst-add-menu.png?width=30pc&classes=shadow)
   *The Add menu of a room.*

3. Choose the template, then fill in the fields. Required fields are marked; the **Créer** (Create) button stays inactive while a field is invalid.
4. Click **Créer**: the device appears in the room, with its IOs below it.

### ESPHome air conditioner

Main fields: the name, the room, the **topic prefix** and the **id** of the entity (see [ESPHome air conditioner]({{% relref "climate/esphome" %}}) to find them), the address and port of the MQTT broker (default `127.0.0.1:1883`), the offered modes, "Two setpoints", the setpoint bounds and step, action reporting, fan and swing modes, and the **room sensor** to relay.

![ESPHome air conditioner wizard](/en/climate/images/inst-wizard-esphome-climate.png?width=36pc&classes=shadow)
*The ESPHome air conditioner template filled in for "Clim salon".*

Defaults: all modes, one setpoint, 16 to 30 °C in 0.5 °C steps, action reported, no fan or swing. When a room sensor is chosen, the device relays its value every 120 s (`remote_temp_keepalive`) and `sensor_timeout` is 9000 s.

### ESPHome infrared air conditioner

Same idea, with the `off`, `heat`, `cool` and `auto` modes, a 1 °C step, and an optional room temperature. An infrared unit has neither action nor remote temperature.

![Infrared air conditioner wizard](/en/climate/images/inst-wizard-esphome-climate-ir.png?width=36pc&classes=shadow)
*The infrared template filled in for "Clim chambre".*

### Heating thermostat

Fields: the name, the room, the **temperature sensor** (an existing IO), the **heating output** (an existing relay) and the **emitter**. The emitter picks the hysteresis and the minimum times (see [Internal thermostat]({{% relref "climate/thermostat" %}})). The device creates its setpoint and its action, which are internal variables.

![Heating thermostat wizard](/en/climate/images/inst-wizard-thermostat-heat.png?width=36pc&classes=shadow)
*The Heating thermostat template filled in for "Radiateur bureau".*

### Heat / cool thermostat

The same with a **cooling output** added. The device also creates a low and a high setpoint for `auto` mode, at least 2 °C apart for a heat pump (1 °C for fan coils).

![Heat cool thermostat wizard](/en/climate/images/inst-wizard-thermostat-heat-cool.png?width=36pc&classes=shadow)
*The Heat / cool thermostat template filled in for "PAC cuisine".*

### Heat / cool floor

Like the previous one, with a mandatory **humidity sensor** and the **maximum humidity** (60 % by default, from 40 to 90 %) from which cooling is cut.

![Floor wizard](/en/climate/images/inst-wizard-thermostat-floor.png?width=36pc&classes=shadow)
*The Heat / cool floor template filled in for "Plancher séjour".*

### One relay, one thermostat only

Output lists do not offer a relay already used by another thermostat: an output has a single owner (see [Internal thermostat]({{% relref "climate/thermostat" %}})). If the relay is taken, the refusal is explained on screen.

![Refused output](/en/climate/images/inst-output-refused.png?width=36pc&classes=shadow)
*The relay is already driven by another thermostat.*

## The device in the tree

The device appears in its room, with its owned IOs below. A right click opens the device menu; **deleting the device also deletes all its IOs**. If an external IO still linked to a device is deleted, or if the deletion affects rules, the installer says so and lists what is concerned before going on.

![Device in the tree](/en/climate/images/inst-tree-device.png?width=30pc&classes=shadow)
*The devices of the demo house: room, device, owned IOs.*

## Editing an existing device

No need to recreate a device to change the sensor or the hysteresis: the **device editor** (right click the device, **Propriétés** / Properties) changes the **links** (for example the room sensor) and the **parameters** (hysteresis, minimum times, modes...) without touching the owned IOs or the rules that use them. The roles and parameters shown come from the server's embedded documentation: each has its label, description, type, bounds and default value.

![Device editor](/en/climate/images/inst-device-editor.png?width=36pc&classes=shadow)
*The device editor of "Clim salon".*

## Sending to the server

Like the rest of the project, the device only exists on the server after the [configuration is sent]({{% relref "getting_started/upload" %}}). Calaos Installer tells the server that it understands devices (`io_format` 2); a server too old for devices refuses the upload rather than receive a file it does not understand.

{{% notice note %}}
The wizards and the editor are evolving: the list of ESPHome climates discovered on the broker, the labels and the IO choice filters are being improved. The screenshots on this page may differ slightly from the version you use.
{{% /notice %}}
