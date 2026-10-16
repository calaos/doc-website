+++
title = "Heating rules"
date = 2026-10-15T09:30:00Z
weight = 50
summary = "Schedule the setpoint, handle absence, open window, frost protection, a common circulator, a reversible floor on a heat pump and domestic hot water, step by step."
+++

A thermostat or air conditioner contains no policy: no schedule, no away mode. That is the job of [rules]({{% relref "calaos_installer/rules" %}}), which **write the device's setpoint or mode**, as they would write any IO. This page gives complete recipes. You will find most of them in [example projects]({{% relref "calaos_installer/devices/examples" %}}) to download and open in Calaos Installer.

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

## 5. A common circulator or boiler (floor heating)

*The circulator runs as soon as a zone asks for heat (or cold), and stops when no zone asks for anything.*

For floor heating alone, each zone is a "Heating thermostat" (emitter "Underfloor heating") whose output is its valve. Each thermostat has **its own output** (a valve or relay per zone; an output can belong to one thermostat only). Rather than guessing what the thermostat is doing, the rules read **those outputs themselves**: they are exactly the relays the regulation drives.

- One rule **per zone output**, for example "Circulateur : Relais radiateur bureau allumé": condition, the output "Relais radiateur bureau" == `true`; action, "Circulateur chauffage" on. Do the same for every heating **and** cooling output of the other zones ("PAC cuisine chaud", "PAC cuisine froid", "Plancher chaud", "Plancher froid").
- One rule "Circulateur : toutes les zones éteintes": one condition per zone output, all == `false`; action, "Circulateur chauffage" off.

The conditions are of the **Condition on output event** kind (the **+** menu of the conditions, after selecting the output in the tree): it is the kind that can read the state of a relay. The "Normal Condition" kind only applies to inputs.

![Circulator rule](/en/calaos_installer/devices/images/inst-rule-circulator.png?width=40pc&classes=shadow)
*The "all zones off" rule: one condition per output (the fifth, "Plancher froid", is below the visible list) and the circulator switched off.*

### Why it works in every mode

An output is on **if and only if** the thermostat is asking for heat or cold at that moment. The mode (`heat`, `cool`, `auto`), the setpoint, the minimum on and off times, the safeties (silent sensor, out-of-range sensor, dew point) and the thermostat's lockouts all decide the state of the output, and the rule only sees the result. It does not need to know the mode: a thermostat in `cool` or `auto`, or cut by a safety, behaves correctly with no extra rule. By contrast, a rule written on the thermostat's "action" IO (`heating`) ignores cooling and runs ahead of the relay whenever a minimum time delays the switching.

These rules never write the outputs, they only read them: the circulator is the only output they drive.

## 6. Reversible floor: nine zones and a heat pump

*The house has nine zones of hydronic floor and a reversible heat pump: in winter it makes hot water, in summer cold water. It is driven by two dry contacts, "heat demand" and "cool demand", which must never be closed together. Each zone has a single electrovalve, the same for heating and cooling. Six zones can cool; the bathroom, the entrance and the laundry room only heat.*

This complete project can be [downloaded]({{% relref "calaos_installer/devices/examples" %}}): you can open it in Calaos Installer while reading this recipe. Its names are in French, as in the screenshots ("PAC" is the heat pump, "Plancher" the floor, "Mode plancher" the floor mode).

### The idea

An open valve lets water through; the heat pump decides whether it is hot or cold. Each zone has its thermostat, which opens its valve when the room needs water: in heat mode when the room is too cold, in cool mode when it is too warm. So the thermostat must have the right mode, and it does not know the mode of the heat pump (see [The zone valve]({{% relref "calaos_installer/devices/thermostat" %}})).

A single setting decides the direction for the whole house: the **"Mode plancher"** variable. Rules copy it into every zone, then turn the right heat pump contact on as soon as a valve is open:

{{< mermaid >}}
graph TD
    M["Mode plancher<br/>0 off, 1 heat, 2 cool"] -->|"rules 'Plancher : mode...'"| Z["9 zone thermostats<br/>heat, cool or off"]
    Z -->|"regulation"| V["9 zone valves<br/>open or closed"]
    V -->|"rules 'PAC chaud / froid'"| P["Heat pump<br/>heat contact or cool contact"]
    M -->|"gives the direction"| P
{{< /mermaid >}}

In summer, set "Mode plancher" to 2: one gesture, and the rules put every zone in cool.

### Step 1: the nine zones

For each zone, add a device with the **Reversible floor zone** template (see [Create a device]({{% relref "calaos_installer/devices/templates" %}})): the zone sensor, **its** valve (one valve per zone: an output has one thermostat only) and the modes.

- The six zones that cool keep **Heating** and **Cooling**: their `modes` parameter is `off,heat,cool`.
- The other three only keep **Heating**: `modes` is `off,heat`. They refuse cool mode.

![Zone that heats and cools](/en/calaos_installer/devices/images/inst-wizard-floor-zone.png?width=36pc&classes=shadow)
*"Plancher séjour": heating and cooling, with a humidity sensor.*

![Heating-only zone](/en/calaos_installer/devices/images/inst-wizard-floor-zone-heat.png?width=36pc&classes=shadow)
*"Plancher salle de bain": heating only.*

Also create the two heat pump relays, "PAC demande chaud" and "PAC demande froid". They are plain outputs: **no thermostat owns them**, only the rules of step 3 drive them.

![The zones in the tree](/en/calaos_installer/devices/images/inst-tree-floor.png?width=30pc&classes=shadow)
*Nine zones, their valves, the two heat pump contacts and the "Mode plancher" variable.*

### Step 2: the mode of the whole house

Create an [internal variable]({{% relref "calaos_installer/internal_var" %}}) of **integer** type, "Mode plancher", initial value 0, with **Save** ticked so that it survives a restart.

| Value | House mode |
|---|---|
| 0 | off |
| 1 | heat |
| 2 | cool |

Three rules copy it into the zones. Here is the summer one, **"Plancher : mode froid"** (floor: cool mode):

- **Condition**: "Mode plancher" == 2.
- **Actions**:
  - the living room, kitchen, office, parents' bedroom, bedroom 1 and bedroom 2 zones receive `cool`;
  - the bathroom, entrance and laundry zones receive `off`: they do not cool, they switch off.

As soon as "Mode plancher" becomes 2, this rule runs and puts the nine zones in the right mode: you do not touch any room. Its two siblings are built the same way:

| Rule | Condition | Actions |
|---|---|---|
| Plancher : arrêt général (all off) | "Mode plancher" == 0 | every zone receives `off` |
| Plancher : mode chaud (heat) | "Mode plancher" == 1 | every zone receives `heat` |
| Plancher : mode froid (cool) | "Mode plancher" == 2 | six zones receive `cool`, the other three `off` |

![Heat mode rule](/en/calaos_installer/devices/images/inst-rule-floor-mode.png?width=40pc&classes=shadow)
*The "Plancher : mode chaud" rule: one condition on the variable, nine actions.*

"Mode plancher" is changed like any IO: make it visible to set it from the interfaces, put it in a "Summer" or "Winter" [scenario]({{% relref "calaos_installer/scenario" %}}), or have a rule change it (calendar, outdoor temperature).

### Step 3: the heat pump contacts

The heat pump must only run when a zone needs water, and in the right direction:

- an open valve means a zone needs water (its thermostat only opens it for that);
- "Mode plancher" gives the direction.

So the rules read the valves, as for the circulator of recipe 5:

| Rule | Conditions | Actions, in this order |
|---|---|---|
| PAC chaud : vanne *zone* ouverte (nine rules) | valve *zone* == `true` **and** "Mode plancher" == 1 | "PAC demande froid" off, then "PAC demande chaud" on |
| PAC froid : vanne *zone* ouverte (six rules, the zones that cool) | valve *zone* == `true` **and** "Mode plancher" == 2 | "PAC demande chaud" off, then "PAC demande froid" on |
| PAC : toutes les vannes fermées (all valves closed) | the nine valves == `false` | "PAC demande chaud" and "PAC demande froid" off |

The conditions on the valves are of the **Condition on output event** kind (as for the circulator), the one on the variable a **Normal condition**.

The two contacts can never be on together:

- a "PAC chaud" rule needs "Mode plancher" == 1, a "PAC froid" rule == 2: the variable has one value at a time;
- each rule first switches the opposite contact off, then its own on;
- no thermostat drives these contacts, only these rules.

![Heat contact rule](/en/calaos_installer/devices/images/inst-rule-floor-heat-contact.png?width=40pc&classes=shadow)
*A valve opens in heat mode: the "cool" contact is switched off, the "heat" contact on.*

![Cool contact rule](/en/calaos_installer/devices/images/inst-rule-floor-cool-contact.png?width=40pc&classes=shadow)
*The same zone in cool mode.*

![All valves closed rule](/en/calaos_installer/devices/images/inst-rule-floor-all-off.png?width=40pc&classes=shadow)
*The nine valves closed: the heat pump has no demand left.*

When going from heat to cool, the contact follows the new mode at once if a valve is still open. The valves opened for heating close at the end of their minimum on time (15 minutes), then reopen for cooling after their minimum off time. To have everything stop first, go through 0: off closes the valves at once.

### Step 4: a zone set by hand to the other direction

In midwinter, someone sets the living room to cool from their phone. Without a rule, the living room would open its valve as soon as it is too warm, the heat pump (in heat) would send hot water, and the room would warm up while trying to cool.

One rule per zone and per direction puts the zone back into the house mode as soon as its mode changes:

| Rule | Conditions | Action |
|---|---|---|
| Plancher *zone* : retour en chaud (back to heat, six zones) | *zone* == `cool` **and** "Mode plancher" == 1 | *zone* receives `heat` |
| Plancher *zone* : retour en froid (back to cool, nine zones) | *zone* == `heat` **and** "Mode plancher" == 2 | *zone* receives `cool` (`off` for a zone that does not cool) |

The condition on *zone* is a **Condition on output event** on the device itself (select the device in the tree, not one of its IOs): the compared value is its mode.

During this round trip, nothing moves on the installation:

- an open valve does not change direction before its minimum on time: it stays open and keeps heating;
- the heat pump contacts follow "Mode plancher", not the zone mode: the heat pump never switches to cool for a single zone.

A zone set to `off` by hand is never touched by these rules.

![Back to heat rule](/en/calaos_installer/devices/images/inst-rule-floor-putback.png?width=40pc&classes=shadow)
*The living room set to cool during the heating season goes back to heat.*

### Frequently asked questions

**I set a zone to cool while the heat pump heats?**
The zone's "retour en chaud" rule puts it back to `heat` at once. The room keeps heating normally. To cool, the whole house goes to cool.

**How do I switch to summer?**
Set "Mode plancher" to 2, from the interfaces, a scenario or a rule. The "Plancher : mode froid" rule puts the six zones in cool and switches the other three off. To go back to winter, set it to 1.

**How do I switch off a single room?**
Set its zone to `off`, from the interfaces or a rule: the put-back rules do not touch it. It stays off until the next change of "Mode plancher", which puts every zone back into the house mode.

**And without a humidity sensor?**
Cooling stays allowed. Without a sensor, the thermostat cannot prevent condensation: the heat pump must keep the water above the dew point. The server reminds you at load with a warning for each zone without a sensor ("cooling has no dew-point cut"): this is expected. A zone **with** a sensor (the living room here) cuts its cooling as soon as humidity reaches `humidity_max`; its valve closes and the heat pump contacts follow, with no extra rule. See [Internal thermostat]({{% relref "calaos_installer/devices/thermostat" %}}).

## 7. Domestic hot water on the same heat pump

*The domestic hot water tank is heated by the same heat pump, but it depends neither on the zones nor on the house mode.*

Hot water is an **independent thermostat**, with **its own** sensor and **its own** relay, that has no link with the nine zones and that none of the rules above reads. It is the hot water demand of the heat pump, which the thermostat switches by itself.

1. Create the tank sensor and the hot water demand relay, then a **Heating thermostat**. The emitter has no equivalent for a tank: choose "Water radiators" (300 s minimum on and off times), then adapt.

   ![Domestic hot water wizard](/en/calaos_installer/devices/images/inst-wizard-dhw.png?width=36pc&classes=shadow)
   *"Eau chaude sanitaire": the tank sensor and the demand relay.*

2. In the **device editor**, change what the templates set for a room:

   | Parameter | Template | For a tank | Why |
   |---|---|---|---|
   | `valid_max` | 60 | 95 | at 60 °C a properly hot tank sensor would be judged "out of range" and cut production |
   | `valid_min` | -30 | 5 | an unplugged or shorted sensor leaves these bounds |
   | `hysteresis` | 0.5 | 3 | a tank has inertia: avoid short restarts |

   ![Hot water thermostat editor](/en/calaos_installer/devices/images/inst-dhw-editor.png?width=36pc&classes=shadow)
   *The parameters of "Eau chaude sanitaire".*

3. Raise the **maximum setpoint**: a thermostat's setpoint is an internal variable limited to 30 °C by default. Open its properties in the tree and set `value_max` to 65 (and the value to 55, for instance): a 55 °C setpoint would otherwise be brought back to 30 °C.

   ![Setpoint properties](/en/calaos_installer/devices/images/inst-dhw-setpoint.png?width=30pc&classes=shadow)
   *The hot water setpoint: bounds raised.*

The demand relay belongs to the thermostat: only write its **mode** in rules (for instance `off` to allow hot water only at certain hours, as in recipe 1). It has nothing to do with "PAC demande chaud" and "PAC demande froid", which only the zones drive.

## Pitfalls

- **Loops**: a rule that writes a setpoint and has that same setpoint in its conditions can call itself indefinitely. Use an internal variable to remember that the action has been done.
- **Contradicting rules**: a schedule and an absence rule write the same setpoint; the last one executed wins. Add the condition "Absence == false" to the schedule rules.
- **After a server restart**, nothing guarantees that the setpoint matches the time: add a rule with a start condition that restores it.
- **A heat pump contact is only driven by rules**: no thermostat owns it, and the "all valves closed" rule must exist, otherwise a contact stays on after the last demand.
