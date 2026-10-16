+++
title = "Internal thermostat"
date = 2026-10-15T09:10:00Z
weight = 30
summary = "Calaos regulates: hysteresis, minimum times, safeties (silent sensor, dew point, out-of-range sensor), one output per thermostat."
+++

## When to use it

The internal thermostat (`thermostat`) is for installations where **nothing regulates for you**: electric radiators behind a relay, a floor heating or cooling valve, a boiler with a contact, a heat pump driven on dry contacts. Calaos reads a temperature sensor, compares it to the setpoint, and switches one or two relays.

If the appliance has its own regulation (split air conditioner, ESPHome heat pump), prefer the relayed kind: see [ESPHome air conditioner]({{% relref "hardware/esphome_climate" %}}).

## What the thermostat does, and what stays in your rules

The **regulation** lives in the thermostat and cannot be changed: it reacts to every change of temperature, setpoint, mode and humidity, and to its timers.

The **policy** stays in your rules: night setback, away mode, open window, schedules. They write the thermostat's setpoint or mode, as they would for any IO. The thermostat has no scheduler of its own; see [Heating rules]({{% relref "calaos_installer/rules_heating" %}}).

## Regulation

The thermostat has a mode (`off`, `heat`, and `cool` and `auto` if it can cool) and one or two setpoints. The offered modes depend on the linked outputs, or on the `modes` parameter for a [zone valve](#the-zone-valve-single-output):

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
| Reversible floor thermostat (two outputs) | 0.5 °C | 900 s | 900 s | 2 °C |
| Reversible floor zone (one valve) | 0.5 °C | 900 s | 900 s | - |

A floor has a lot of inertia: a wide hysteresis and long times avoid jerks. For all thermostat templates `sensor_timeout` is 9000 s, `put_back_delay` 1 s (maximum 60 s), and the accepted readings go from -30 to 60 °C (see [Sensor out of range](#sensor-out-of-range)). All these values can be changed afterwards in the device editor.

## Safeties

A thermostat that drives relays must never stay on because of wrong information. In all the cases below **the outputs are switched off**, the mode and setpoint stay adjustable, and regulation resumes by itself when the situation returns to normal.

| Situation | What is displayed | Detail |
|---|---|---|
| At startup, the sensor has not read anything yet | "Waiting for the sensor" | No regulation until the sensor has delivered a value since startup. A cached value or one retained by MQTT does not count. |
| The sensor has sent nothing for `sensor_timeout` seconds | "Sensor silent" | Even a repeated identical value counts as a reading. 9000 s by default, above a missed hourly Zigbee report. |
| The sensor reports itself disconnected | "Sensor disconnected" | Connection link of the sensor (MQTT, Wago...). |
| The `current_temp` link does not exist | "Sensor missing" | Misconfigured device. |
| Floor with a humidity sensor: humidity too high, silent or never read | "Humidity too high" or "Humidity sensor silent" | See the dew point below. |
| The reading is outside the plausibility bounds, or is not a number | "Sensor out of range" | See [Sensor out of range](#sensor-out-of-range). |

When several situations add up, the interface shows only one, in priority order: sensor missing, waiting, silent, disconnected, out of range, then the humidity faults.

### Dew point (cooled floor)

Cooling a floor below the dew point makes the humidity of the air **condense** on the floor. With the "Floor" emitter (`emitter` = `floor`) and a cooling mode, the thermostat can therefore watch a **humidity sensor** (`humidity` link): it cuts cooling while humidity reaches `humidity_max` (60 % in the templates, 65 % if the parameter is missing), or while the sensor is silent, disconnected or has never read.

The humidity sensor is **optional**:

- **with** a linked sensor the cut applies, in `cool` mode as in `auto` mode;
- **without** a sensor, cooling is allowed and the thermostat has no protection against condensation: the installation must keep the water above the dew point (the heat pump, for example). The server flags it with a warning in the log at load;
- a `humidity` link to an IO that cannot be found counts as a linked sensor: cooling stays cut rather than being allowed by a typo.

Only the "Floor" emitter is concerned: with another emitter, humidity is not read. The "Reversible floor thermostat" template (two outputs) always asks for a humidity sensor; the "Reversible floor zone" template (one valve) leaves it optional.

### Sensor out of range

A sensor can lie without going silent: a DS18B20 reads 85 °C at power-on and -127 °C when unplugged, and a Wago sensor with a cut or shorted cable reads an absurd value instead of reporting itself disconnected. Two thermostat parameters, `valid_min` and `valid_max`, bound the accepted readings (bounds included); a missing bound is not checked.

A reading outside the bounds counts as **no valid reading**:

- the outputs are switched off **at once**, without waiting for `min_on_time`; `min_off_time`, however, still delays the restart;
- the thermostat shows **"Sensor out of range"**, and its mode and setpoints stay adjustable;
- regulation resumes at the first reading within the bounds;
- a reading that is not a number (NaN, infinity) is always out of range, even without bounds;
- there is **no hysteresis** on the bounds: a sensor oscillating around a bound makes the outputs cycle at the pace of `min_off_time`;
- if `valid_max` is below `valid_min`, both bounds are ignored (a message in the log).

The thermostat templates write `valid_min` = -30 and `valid_max` = 60: this covers the absurd values above without bothering a frost-free garage. The ESPHome climate templates have no bounds. Change them in the device editor according to what the sensor measures: `valid_min` = 5 and `valid_max` = 40 for a living room, but a domestic hot water tank sensor must go above 60 °C (see [Heating rules]({{% relref "calaos_installer/rules_heating" %}})).

## The zone valve (single output)

### The situation

A reversible floor heats in winter and cools in summer. Each zone (a room or a group of rooms) has **one electrovalve** on its circuit:

- valve open, water flows through the floor of the zone;
- valve closed, it does not.

The valve does not know whether the water is hot or cold. The **heat pump** decides that, for the whole house.

So the zone thermostat does not have two outputs, "heating" and "cooling", but **a single one**: the **Zone valve** (`output` link). The **Reversible floor zone** template of the wizard creates the thermostat that way.

### What the zone thermostat does

| Zone mode | The valve opens when... |
|---|---|
| `heat` | the room is **below** the setpoint |
| `cool` | the room is **above** the setpoint |
| `off` | never: the valve stays closed |

The valve closes again once the setpoint is reached.

So the thermostat needs **its own mode** to know when to open. It does not know the mode of the heat pump: nothing tells it whether the pump makes hot or cold water. If the pump heats and a zone is in cool, that zone opens its valve when the room is too warm... and gets hot water.

That is why a reversible installation has **one mode for the whole house**, and rules that give it to every zone and to the heat pump. The full recipe is in [Heating rules]({{% relref "calaos_installer/rules_heating" %}}), recipe 6, and a [ready-to-open example]({{% relref "calaos_installer/devices/examples" %}}) contains it already built.

### Zones that do not cool

The `modes` parameter lists the modes the zone accepts, besides `off`:

- a zone that heats and cools carries `modes` = `off,heat,cool`;
- a bathroom that is never cooled carries `modes` = `off,heat`. It refuses `cool`: the request is ignored and noted in the log;
- without this parameter, the zone only offers heating, for safety (the log says so);
- `auto` (heat or cool according to a low and a high setpoint) is only possible when `heat` and `cool` are both in the list.

### Details

- A zone valve replaces `output_heat` and `output_cool`: a thermostat linking both is refused at load (a message in the log), and the valve is ignored.
- Like any thermostat relay, a valve belongs to a single zone.
- When the zone mode changes direction (heat to cool, or back), an open valve only closes at the end of its minimum on time (`min_on_time`, 15 minutes in the template). It only reopens in the new direction after its minimum off time (`min_off_time`). The interfaces show "Starts in N min". Mode `off` closes the valve at once.
- The displayed action follows the mode: "heating" or "cooling".
- With a linked humidity sensor, cooling is cut as soon as humidity exceeds `humidity_max`, a valve waiting for its change of direction included (see [Dew point](#dew-point-cooled-floor)).

## One output, one thermostat

A relay linked as heating or cooling output belongs to **one** thermostat only. Two thermostats writing the same relay would fight: one turns it on, the other off. Calaos therefore refuses the second link at load (a message in the log), and the wizard does not offer an output that is already taken.

![An output already taken, refused by the editor](/en/calaos_installer/devices/images/inst-output-refused.png?width=30pc&classes=shadow)
*A relay already used by another thermostat is refused.*

For multi-zone heating sharing a boiler, a circulator or a heat pump, create **one output per zone** (a valve or relay per thermostat), and **rules** that turn the common output on as soon as a zone output is on. See the examples in [Heating rules]({{% relref "calaos_installer/rules_heating" %}}).

## Outputs are protected

The relays driven by a thermostat must not be written elsewhere. If a rule, a Lua script or a command writes one directly, the value is applied **but a warning is logged**: Calaos never blocks the rules engine. The thermostat then puts the output back to the wanted state after `put_back_delay` seconds (the delay doubles while the other writer keeps undoing it, up to `put_back_delay_max`), and logs every put-back as an error. The MCP assistant does not see these outputs and cannot write them.

To switch a thermostat off, write its **mode** (`off`), never its relay.

## The thermostat in the interfaces

The displayed status combines the action (heating, cooling, idle), the minimum-time lock-out and the faults above: see [Display and control]({{% relref "calaos_installer/devices/clients" %}}).
