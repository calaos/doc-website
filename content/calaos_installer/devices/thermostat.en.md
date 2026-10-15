+++
title = "Internal thermostat"
date = 2026-10-15T09:10:00Z
weight = 30
summary = "Calaos regulates: hysteresis, minimum times, safeties (silent sensor, dew point, out-of-range sensor), one output per thermostat."
+++

## When to use it

The internal thermostat (`thermostat`) is for installations where **nothing regulates for you**: electric radiators behind a relay, a floor heating valve, a boiler with a contact, a heat pump driven on dry contacts. Calaos reads a temperature sensor, compares it to the setpoint, and switches one or two relays.

If the appliance has its own regulation (split air conditioner, ESPHome heat pump), prefer the relayed kind: see [ESPHome air conditioner]({{% relref "hardware/esphome_climate" %}}).

## What the thermostat does, and what stays in your rules

The **regulation** lives in the thermostat and cannot be changed: it reacts to every change of temperature, setpoint, mode and humidity, and to its timers.

The **policy** stays in your rules: night setback, away mode, open window, schedules. They write the thermostat's setpoint or mode, as they would for any IO. The thermostat has no scheduler of its own; see [Heating rules]({{% relref "calaos_installer/rules_heating" %}}).

## Regulation

The thermostat has a mode (`off`, `heat`, and `cool` and `auto` if it has a cooling output) and one or two setpoints:

| Mode | Behaviour |
|---|---|
| `off` | both outputs are off |
| `heat` | heating **on** below `setpoint - hysteresis`, **off** as soon as the setpoint is reached |
| `cool` | cooling **on** above `setpoint + hysteresis`, **off** as soon as the setpoint is reached |
| `auto` | heating follows the **low** setpoint, cooling the **high** one; in between, nothing runs |

With a 21 °C setpoint and a 0.5 °C hysteresis, heating turns on at 20.5 °C and off at 21 °C. The hysteresis keeps a relay from clicking at every tenth of a degree.

### Minimum times

`min_on_time` and `min_off_time` set, for each output, the minimum time on and off. A switch requested during that time is **deferred** until it expires, then the regulation is evaluated again. The interfaces then show "Starts in N min". These times protect a compressor or a thermal valve from switching too fast.

In `auto` mode the two outputs are **never** on together, and a changeover from heat to cool also waits for the minimum off time of the other output. The `min_range` parameter enforces a minimum gap between the low and the high setpoint; a closer range is refused.

### Template defaults

The template proposes values suited to what the outputs drive (the **Emitter** choice in the wizard):

| Template, emitter | Hysteresis | Min on | Min off | Min gap |
|---|---|---|---|---|
| Heating thermostat, electric radiators | 0.3 °C | 120 s | 120 s | - |
| Heating thermostat, water radiators | 0.5 °C | 300 s | 300 s | - |
| Heating thermostat, floor heating | 0.5 °C | 900 s | 900 s | - |
| Heat / cool, reversible heat pump on contacts | 0.5 °C | 600 s | 300 s | 2 °C |
| Heat / cool, fan coils | 0.5 °C | 120 s | 120 s | 1 °C |
| Heat / cool floor | 0.5 °C | 900 s | 900 s | 2 °C |

A floor has a lot of inertia: a wide hysteresis and long times avoid jerks. For all templates `sensor_timeout` is 9000 s and `put_back_delay` 1 s (maximum 60 s). All these values can be changed afterwards in the device editor.

## Safeties

A thermostat that drives relays must never stay on because of wrong information. In all the cases below **the outputs are switched off**, the mode and setpoint stay adjustable, and regulation resumes by itself when the situation returns to normal.

| Situation | What is displayed | Detail |
|---|---|---|
| At startup, the sensor has not read anything yet | "Waiting for the sensor" | No regulation until the sensor has delivered a value since startup. A cached value or one retained by MQTT does not count. |
| The sensor has sent nothing for `sensor_timeout` seconds | "Sensor silent" | Even a repeated identical value counts as a reading. 9000 s by default, above a missed hourly Zigbee report. |
| The sensor reports itself disconnected | "Sensor disconnected" | Connection link of the sensor (MQTT, Wago...). |
| The `current_temp` link does not exist | "Sensor missing" | Misconfigured device. |
| Floor: humidity too high, silent or never read | "Humidity too high" or "Humidity sensor silent" | See the dew point below. |
| The reading is outside the plausibility bounds | "Sensor out of range" | See below (feature under development). |

### Dew point (cooled floor)

Cooling a floor below the dew point makes the humidity of the air **condense** on the floor. With the "Floor" emitter and a cooling output, Calaos therefore requires a `humidity` link and cuts cooling while humidity reaches `humidity_max` (60 % by default in the "Heat / cool floor" template, 65 % if the parameter is missing), or while the humidity sensor is silent or has never read.

**Without a humidity sensor, a floor has neither `cool` nor `auto` mode**: they are refused at load (the refusal is logged) and not offered in the interfaces.

### Sensor out of range

{{% notice warning %}}
This feature (ticket CLIM-9) is **under development**: the names below may still change before release.
{{% /notice %}}

A sensor can lie without going silent: a Wago sensor with a cut cable sometimes reads an absurd value (-50 °C, 0 °C) instead of reporting itself disconnected. Two thermostat parameters, `valid_min` and `valid_max`, bound the accepted readings. Without these parameters no check is made.

A reading outside the bounds counts as **no valid reading**: the outputs are switched off, the thermostat shows **"Sensor out of range"**, mode and setpoint stay adjustable, and everything returns to normal at the first reading within the bounds. The bounds of a room, a floor and hot water are not the same; the templates propose some. Example for a room: `valid_min` = 5 and `valid_max` = 40.

## One output, one thermostat

A relay linked as heating or cooling output belongs to **one** thermostat only. Two thermostats writing the same relay would fight: one turns it on, the other off. Calaos therefore refuses the second link at load (a message in the log), and the wizard does not offer an output that is already taken.

![An output already taken, refused by the editor](/en/calaos_installer/devices/images/inst-output-refused.png?width=30pc&classes=shadow)
*A relay already used by another thermostat is refused.*

For multi-zone heating sharing a boiler or a circulator, create **one output per zone** (a valve or relay per thermostat), and **one rule** that turns the common output on as soon as a zone output is on. See the example in [Heating rules]({{% relref "calaos_installer/rules_heating" %}}).

## Outputs are protected

The relays driven by a thermostat must not be written elsewhere. If a rule, a Lua script or a command writes one directly, the value is applied **but a warning is logged**: Calaos never blocks the rules engine. The thermostat then puts the output back to the wanted state after `put_back_delay` seconds (the delay doubles while the other writer keeps undoing it, up to `put_back_delay_max`), and logs every put-back as an error. The MCP assistant does not see these outputs and cannot write them.

To switch a thermostat off, write its **mode** (`off`), never its relay.

## The thermostat in the interfaces

The displayed status combines the action (heating, cooling, idle), the minimum-time lock-out and the faults above: see [Display and control]({{% relref "calaos_installer/devices/clients" %}}).
