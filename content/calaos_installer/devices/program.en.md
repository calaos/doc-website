+++
title = "Weekly program"
date = 2026-11-26T09:00:00Z
weight = 35
summary = "Schedule a thermostat or air conditioner for the week: slots, temperature levels, priorities, copying a day, pausing, and where to edit."
+++

A climate can carry its own **weekly program**, with no rules to write. It requires at least calaos_base 4.9.0, calaos-web-app 4.3.0 and calaos_mobile 4.6.0. Heating (`heat`) and cooling (`cool`) each have their own program, because their schedules and setpoints differ.

## Slots and levels

- A **slot** is a time range on one day of the week, with **its own temperature**. Several slots can share the same temperature.
- For one program, a week holds at most **10 different temperatures**. Each temperature is a **level**, numbered 1 to 10.
- A slot that crosses midnight is split in two: for example Thursday 22:30 to 24:00, then Friday 00:00 to 06:00.
- Setpoints range from 7 to 30 °C, in 0.5 steps.

### Priorities

When two slots overlap, the **lowest level wins**: level 1 beats level 2. The level number is the priority: in a slot's form, move the level up or down to change it.

### Default setpoint

Each program has a **default setpoint**, applied when no slot is active. For cooling you can choose **"Cooling off outside periods"**: outside the slots, the setpoint is pushed to the maximum (30 °C) and the mode stays `cool`.

## What the program does

The program generates no rules. The device writes its own setpoint:

- when a slot starts or ends;
- when the mode changes;
- when you enable or pause the program;
- when you change a level's setpoint;
- when the server starts.

A setpoint set by hand **holds until the next of these events**.

| Mode | Effect of the program |
|---|---|
| `heat`, `cool` | writes the active level's setpoint |
| `auto` | writes the low setpoint from the heating program and the high one from the cooling program; the minimum gap is respected (2 °C by default) |
| `off`, `dry`, `fan_only` | writes nothing |

## Pausing

A **Program enabled** switch exists for each climate. While paused, the last setpoint stays in place and screens show "Program paused: period (xx°)". On resume, the current slot is applied again. The switch can also be read and driven by a [rule]({{% relref "calaos_installer/rules" %}}).

## Editing the program

### Web app, mobile app and wall display

Open the climate, then the program edit button (**Edit program** on the web, **Program and settings** on phones). Choose the **Heating** or **Cooling** tab, then the day (Mon to Sun). A 24 h strip shows the day's slots.

- **Add a period** adds a slot: start time, end time, temperature. The time field moves in 15-minute steps; typing to the minute is still possible.
- On desktop and on the wall display you can also **drag on the strip** to create a selection, in 15-minute steps. The web app has no drag.
- An 11th different temperature is refused: "This week already uses 10 different temperatures. Choose one of them:", with the existing temperatures to tap.
- **Copy to…** copies the displayed day to other days, for the active tab only. The copy **replaces** the target days' slots; a confirmation is asked if one of them already has slots. An empty source day is refused.
- In `auto` mode, the "i" help explains the two programs; the hatched zone between the two setpoints is the minimum gap.

A read-only session (service or offline) cannot change anything.

Remote control screens (ESP32) only display the state, for example "Level 2 until 22:30 · 20.5°", "Program paused…" or "Cooling off until 12:00". They cannot edit.

The climate row shows a badge for the next change, for example "▶ 17:30".

## In Calaos Installer

Open the thermostat, then **Time program of…** (the weekly-program dialog). The **Program** checkbox (in Heating and/or the Cooling group; tick the modes your heat pump offers) enables the weekly program. Unchecking it **deletes** the program's setpoints, slots and levels. The number of levels is no longer a setting: it is 10 as soon as the program is enabled.

- The program's IOs belong to the device and sit in the **Program** sub-node.
- After adding a program to an existing device, **restart the server**: it creates the IOs.
- In a rule, the selector offers for example "Living room › active program".
- Months set in Calaos Installer are kept when edits are made from the apps.

The MCP assistant refuses to write the program IOs.

## Program or rules?

The program covers typical weeks. Keep [heating rules]({{% relref "calaos_installer/rules_heating" %}}) for the rest: away mode, open window, frost protection.
