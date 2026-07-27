+++
title = 'Time scheduling'
date = 2024-03-06T20:12:55Z
weight = 50
summary = "Triggering on the clock, on the sun, or after a delay."
+++

## Three tools, three needs

Calaos offers three different objects for everything time-related. Picking the right one from the start saves needlessly complicated rules.

| Object | Answers the question | Example |
|---|---|---|
| **Time range** | *Are we within a period?* | "at night", "weekdays from 7 to 9 am" |
| **Schedule** | *What time is it?* | "every day at 10 pm" |
| **Timer** | *How long since…* | "switch off 5 minutes later" |

All three are added from the **Add** menu, and are then used in rules like any other IO.

## The time range

A time range is **true or false** depending on the moment. It triggers nothing by itself: it serves as a **condition** in your rules.

This is the tool to use whenever an automation should only apply at certain times: the motion detector only lights the hallway at night, the heating only warms up in the morning.

For each day of the week you define the periods where the range is active. The boundaries can be fixed times, but also **sunrise and sunset**, which naturally follows the seasons.

{{% notice warning %}}
Sunrise and sunset times depend on where your home is. If you have not set that, Calaos uses **the coordinates of Paris** by default, and your shutters will close at Parisian time.

This is a one-off setting: see [Date & Time]({{% relref "calaos_os/configuration/date" %}}).
{{% /notice %}}

## The schedule

A schedule triggers **at a precise instant**. It is the equivalent of an alarm clock: on that date, that day, that time.

This is what you use for one-off and repeating actions: water at 6 am, run the "Night" scenario at 11 pm, reset a counter at midnight.

## The timer

A timer counts down a delay, then changes state — which lets a rule react when it expires.

Two options determine its behaviour:

| Option | Effect |
|---|---|
| **Auto start** | The timer starts as soon as the server starts |
| **Auto restart** | It starts again from the beginning on every expiry |

With auto restart you get a **regular beat**: an action every ten minutes, a check every hour. Without it, the timer is single-use and must be restarted by a rule.

{{% notice tip %}}
The timer is the answer to "switch off automatically after a while": one rule starts the timer when the light comes on, another switches the light off when it expires. This is the classic setup for a garage or cellar light people forget to switch off.
{{% /notice %}}

## Choosing between the three

The most frequent confusion is between the time range and the schedule.

**"At 10 pm, close the shutters"** — that is a schedule: an event, at an instant.

**"At night, if someone passes, switch on"** — that is a time range: a state, over a period.

A good test: if your sentence contains "at" followed by a time, it is a schedule. If it contains "during", "at night", "on weekdays", it is a time range.

## The server clock

All of this machinery relies on the server's clock. A wrong time produces confusing symptoms: shifted rules, shutters closing at the wrong moment, wall screens refusing to connect.

The server sets its clock automatically over the Internet. If in doubt, see [Date & Time]({{% relref "calaos_os/configuration/date" %}}).

## See also

- [Create rules]({{% relref "calaos_installer/rules" %}}) — to use these objects
- [Scenario]({{% relref "calaos_installer/scenario" %}}) — to trigger at a set time
