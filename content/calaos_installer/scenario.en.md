+++
title = 'Scenario'
date = 2024-03-06T20:12:47Z
weight = 40
summary = "Chaining several actions and triggering them in one go."
+++

## What it is for

A **scenario** groups a series of actions under a single name. Instead of switching off eight lights, closing the shutters and stopping the music one by one, you trigger "Leaving".

The classic uses:

- **Leaving** — switch everything off, close the shutters, arm the monitoring;
- **Night** — switch off the ground floor, dim the hallway;
- **Cinema** — dim the lights, close the shutters, switch on the projector;
- **Wake up** — open the shutters, switch on the bathroom heating.

A scenario is an IO like any other: it appears in the interfaces, and your rules can trigger it.

## Scenario or rule?

The two look alike, and the confusion is common.

| | Role | Triggers |
|---|---|---|
| **Rule** | React to something | Automatically, when its conditions are true |
| **Scenario** | Perform a series of actions | When asked — button, rule, application |

In other words: the **rule decides when**, the **scenario describes what to do**. The two combine very well — a time rule triggering the "Night" scenario reads better than the same rule holding fifteen actions.

{{% notice tip %}}
As soon as a series of actions must be startable **from several places** — a wall button, the application, a time rule — make it a scenario. You will describe it only once, and a change will benefit every trigger.
{{% /notice %}}

## Creating a scenario

In the **Add** menu, choose **Scenario**, give it a name, then compose the list of actions: for each device concerned, the state to apply.

The **Visible** option determines whether it appears in the interfaces. A scenario meant to be called only by rules can stay hidden.

## Triggering a scenario

Three ways, all equivalent from the server's point of view:

- **from an interface** — the user taps the scenario in the application or on the wall screen;
- **from a rule** — the scenario becomes an action of that rule;
- **from a physical button** — a rule ties the switch to the scenario.

The same scenario can of course be triggered by all three.

## Designing scenarios well

**Stick to a clear intent.** A scenario named "Leaving" must do what its name announces. A catch-all scenario becomes impossible to change with confidence.

**Beware of scenarios calling each other.** It is possible, but a circular chain — "Night" calls "Ground floor" which calls "Night" back — produces behaviour that is hard to diagnose.

**Think about the way back.** If you create "Cinema", plan how to return to a normal state: a second scenario, or a rule restoring the lighting when the projector switches off.

## See also

- [Create rules]({{% relref "calaos_installer/rules" %}}) — to trigger your scenarios automatically
- [Time scheduling]({{% relref "calaos_installer/time" %}}) — to trigger them at a set time
- [Internal Variable]({{% relref "calaos_installer/internal_var" %}}) — to remember which scenario is active
