+++
title = "Heating rules"
date = 2026-10-15T09:30:00Z
weight = 50
summary = "Schedule the setpoint, handle absence, open window, frost protection, and switch a common circulator, step by step."
+++

A thermostat or air conditioner contains no policy: no schedule, no away mode. That is the job of [rules]({{% relref "calaos_installer/rules" %}}), which **write the device's setpoint or mode**, as they would write any IO. This page gives complete recipes.

## The command in a rule action

In a rule, drag the device (not its owned IOs) into the actions area. Calaos Installer shows a **form** instead of free text: one field per command the device accepts (mode, setpoint, low and high setpoint, fan, swing). Only the fields whose link exists on this device are offered; bounds and value lists come from the device.

![Device command form in an action](/en/calaos_installer/devices/images/inst-action-form.png?width=36pc&classes=shadow)
*An action on "Radiateur bureau": Heat mode, 21 °C setpoint.*

The stored value is text:

| You want to... | Action value |
|---|---|
| switch off | `off` |
| heat | `heat` |
| change only the setpoint | `{"target_temp":17}` |
| mode and setpoint together | `{"mode":"heat","target_temp":21}` |
| set a range in auto mode | `{"mode":"auto","target_temp_low":20,"target_temp_high":25}` |
| fan | `{"fan_mode":"low"}` |

Only the fields present are applied, and an invalid field is refused and logged without blocking the others. A button switches to text entry if you prefer to write the JSON yourself. The value cannot be "that of another IO": to copy a value into a setpoint, the rule writes the setpoint IO directly.

{{% notice warning %}}
Never command a thermostat's **relay** in a rule: write its mode (`off`, `heat`). The thermostat would put the relay back to the wanted state, and the log would fill with warnings.
{{% /notice %}}

## 1. Schedule the setpoint

*The office is heated to 21 °C from 7:30 to 18:00 on weekdays, to 17 °C the rest of the time.*

1. Create a **time range** "Bureau occupé" (Monday to Friday, 7:30 to 18:00): see [Time programming]({{% relref "calaos_installer/time" %}}).
2. Rule "Bureau : confort en journée": condition, the "Bureau occupé" range is **true**; action, "Radiateur bureau" receives `{"mode":"heat","target_temp":21}`.
3. Rule "Bureau : réduit hors journée": condition, the range is **false**; action, `{"target_temp":17}`.

![A complete rule](/en/calaos_installer/devices/images/inst-rule-complete.png?width=40pc&classes=shadow)
*The rule "Bureau : confort en journée": one condition, one action.*

Consider a **startup rule** (start condition) if you want the setpoint to be right after a server restart.

## 2. Absence and return

*When nobody is home, all heating drops to 16 °C; on return, it goes back up.*

1. Create a boolean **internal variable** "Absence".
2. Rule "Absence : abaisser le chauffage": condition, "Absence" == true; actions:
   - Radiateur bureau: `{"mode":"heat","target_temp":16}`
   - PAC cuisine: `{"mode":"auto","target_temp_low":16,"target_temp_high":28}`
   - Plancher séjour: `{"mode":"heat","target_temp":16}`
3. Rule "Retour : rétablir": condition, "Absence" == false; actions: `{"target_temp":21}` on the office and the floor, `{"target_temp_low":20,"target_temp_high":25}` on the heat pump.

A rule's actions run in order. To reuse them from a button or a schedule, group them in an "Absence" [scenario]({{% relref "calaos_installer/scenario" %}}) and put the scenario in the action.

## 3. Open window

*If the office window is open, the radiator stops.*

- Rule "Fenêtre bureau ouverte": condition, "Fenêtre bureau" == true; action, "Radiateur bureau" receives `off`.
- Rule "Fenêtre bureau fermée": condition, "Fenêtre bureau" == false; action, `heat`.

The setpoint is untouched: on closing, the thermostat resumes where it was.

## 4. Frost protection

*At night, if the living room drops below 10 °C, the air conditioner heats to 17 °C.*

Rule "Hors-gel : nuit d'hiver": conditions, the "Nuit" range is true **and** "Température salon" < 10; action, "Clim salon" receives `{"mode":"heat","target_temp":17}`. All conditions of a rule must be true.

For permanent frost protection (house closed for the winter), set the setpoint to 7 °C in `heat` mode: it is the default lower bound of a thermostat.

## 5. A common circulator or boiler

*The circulator runs as soon as a zone asks for heat (or cold), and stops when no zone asks for anything.*

Each thermostat has **its own output** (a valve or relay per zone; an output can belong to one thermostat only). Rather than guessing what the thermostat is doing, the rules read **those outputs themselves**: they are exactly the relays the regulation drives.

- One rule **per zone output**, for example "Circulateur : Relais radiateur bureau allumé": condition, the output "Relais radiateur bureau" == `true`; action, "Circulateur chauffage" on. Do the same for every heating **and** cooling output of the other zones ("PAC cuisine chaud", "PAC cuisine froid", "Plancher chaud", "Plancher froid").
- One rule "Circulateur : toutes les zones éteintes": one condition per zone output, all == `false`; action, "Circulateur chauffage" off.

The conditions are of the **Condition on output event** kind (the **+** menu of the conditions, after selecting the output in the tree): it is the kind that can read the state of a relay. The "Normal Condition" kind only applies to inputs.

![Circulator rule](/en/calaos_installer/devices/images/inst-rule-circulator.png?width=40pc&classes=shadow)
*The "all zones off" rule: one condition per output (the fifth, "Plancher froid", is below the visible list) and the circulator switched off.*

### Why it works in every mode

An output is on **if and only if** the thermostat is asking for heat or cold at that moment. The mode (`heat`, `cool`, `auto`), the setpoint, the minimum on and off times, the safeties (silent sensor, out-of-range sensor, dew point) and the thermostat's lockouts all decide the state of the output, and the rule only sees the result. It does not need to know the mode: a thermostat in `cool` or `auto`, or cut by a safety, behaves correctly with no extra rule. By contrast, a rule written on the thermostat's "action" IO (`heating`) ignores cooling and runs ahead of the relay whenever a minimum time delays the switching.

These rules never write the outputs, they only read them: the circulator is the only output they drive.

## Pitfalls

- **Loops**: a rule that writes a setpoint and has that same setpoint in its conditions can call itself indefinitely. Use an internal variable to remember that the action has been done.
- **Contradicting rules**: a schedule and an absence rule write the same setpoint; the last one executed wins. Add the condition "Absence == false" to the schedule rules.
- **After a server restart**, nothing guarantees that the setpoint matches the time: add a rule with a start condition that restores it.
