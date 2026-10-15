+++
title = "How a device works"
date = 2026-10-15T09:05:00Z
weight = 10
summary = "Owned IOs, links, parameters, capabilities: how Calaos represents an air conditioner or a thermostat."
+++

## One appliance, not twenty IOs

An ESPHome air conditioner publishes about twenty values over MQTT: mode, measured temperature, setpoint, fan, swing, connection state... With no other tool you have to create each of them as an MQTT IO, find them again in the tree, and not forget any when you delete the appliance.

A **device** solves this: it is one object that groups all the IOs of one appliance. It is created in one go from a template (see [Create a device]({{% relref "climate/templates" %}})), it shows up as **one piece of equipment** in every interface, and it disappears **with all its IOs** when you delete it.

For now Calaos knows one device family, **climate** (`climate`), with two behaviours:

| Behaviour | Who regulates | Examples |
|---|---|---|
| `proxy` | the appliance itself; Calaos relays its state and commands | Mitsubishi or Fujitsu split through ESPHome, infrared air conditioner |
| `thermostat` | Calaos, by switching relays | electric radiators, floor heating, heat pump on dry contacts, fan coils |

## The three ingredients

### Owned IOs

These are ordinary IOs declared **inside** the device: the MQTT topic of the mode, of the setpoint, the internal variable that keeps a thermostat's setpoint... The device creates and deletes them. They are hidden in the interfaces: you only see the device.

### Links

A **link** gives a **role** to an IO: "this IO is the room temperature", "that one is the setpoint", "this one is the heating relay". The target of a link is either an owned IO or an **external IO** declared elsewhere in the project: the living room temperature sensor, a Wago relay. The device never creates or deletes an external IO.

| Role | Meaning |
|---|---|
| `mode` | operating mode (off, heat, cool...) |
| `current_temp` | room temperature |
| `target_temp` | single setpoint |
| `target_temp_low`, `target_temp_high` | low and high setpoint (the range of `auto` mode) |
| `action` | what the appliance is doing: heating, cooling, idle... |
| `fan_mode`, `swing_mode` | fan and swing |
| `remote_temp`, `remote_temp_source` | room temperature relayed to the unit (see [ESPHome air conditioner]({{% relref "climate/esphome" %}})) |
| `humidity` | room humidity |
| `output_heat`, `output_cool` | heating and cooling relays of a thermostat |

### Parameters

A **parameter** is a setting of the device: hysteresis, minimum on and off times, offered modes, delay before a sensor is declared silent... Default values come from the template; the [device editor]({{% relref "climate/templates" %}}) lets you change them afterwards.

## Capabilities come from links

What the device can do depends **only** on the links that exist. No `fan_mode` link: no fan setting. No `target_temp_low` and `target_temp_high` links: no range in `auto` mode. No `output_cool` link on a thermostat: no cool mode.

The interfaces read these capabilities and only show what exists. An air conditioner without swing has no swing setting, and a radiator thermostat has no `cool` mode to offer.

## What rules see

The rules engine has not changed: it only handles simple values. The value of a device is its **mode**: `off`, `heat`, `cool`, `auto`, `dry` or `fan_only`. A condition "Clim salon == `off`" therefore works on any climate device.

For setpoints, temperature and action, a rule reads the linked IOs, which are ordinary IOs.

## Commanding a device: one command, two forms

Whether it comes from a rule, a Lua script, an app or the MCP assistant, a command sent to a device is text, in one of two forms:

- **a mode alone**: `off` or `heat`;
- **a JSON object**, which changes several fields at once: `{"mode":"heat","target_temp":21.5}`.

| Field | Values |
|---|---|
| `mode` | one of the modes the device offers |
| `target_temp` | single setpoint, bounded and rounded to the device step |
| `target_temp_low`, `target_temp_high` | setpoint range (`auto` mode) |
| `fan_mode`, `swing_mode` | one of the device's fan or swing modes |

Only the fields present are applied. Each field is checked against the capabilities of the instance: a field that does not exist or a value outside the list is **refused and logged**, and the other fields are applied anyway. Numbers are clamped to the bounds and rounded to the step.

In Calaos Installer, a rule action that targets a device shows a **form** instead of free text (see [Heating rules]({{% relref "climate/rules" %}})).

## What it looks like in io.xml

In the configuration file, a device is a `<calaos:device>` element, child of a room, that holds its IOs, links and parameters. Shortened example of a thermostat:

```xml
<calaos:device id="bureau_heat" type="climate" impl="thermostat" name="Radiateur bureau" visible="true">
  <calaos:internal id="bureau_heat_target_temp" type="InternalInt" save="true" visible="false" />
  <calaos:internal id="bureau_heat_action" type="InternalString" visible="false" />

  <calaos:link name="current_temp" io="bureau_temp" />        <!-- external IO -->
  <calaos:link name="output_heat"  io="bureau_radiateur" />   <!-- external IO -->
  <calaos:link name="target_temp"  io="bureau_heat_target_temp" />
  <calaos:link name="action"       io="bureau_heat_action" />

  <calaos:param name="emitter" value="electric" />
  <calaos:param name="hysteresis" value="0.3" />
  <calaos:param name="min_on_time" value="120" />
  <calaos:param name="min_off_time" value="120" />
</calaos:device>
```

A file that contains devices has `format="2"` on `<calaos:home>`. This marking protects your devices: the server **refuses** a configuration sent by a Calaos Installer that does not know them, instead of silently erasing them.
