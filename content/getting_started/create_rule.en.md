+++
title = 'Create a rule'
date = 2024-03-06T19:49:17Z
weight = 40
+++

## Rules in Calaos

In Calaos, the operation is based on rules. A rule consists of one or more conditions that must be met (i.e., be true) in order to execute a list of actions.

## Creating our First Rule

To create a rule, click on the **+** button located above the list of rules. You will then need to give a name and a type to categorize this rule. Next, use the drag-and-drop function to place the switch in the conditions section of the rule, and do the same with the light in the actions section.

![rule](/en/getting_started/images/add_rule.png?width=20pc&classes=shadow)

In this example, this rule will turn the light on or off as soon as you press the switch. This works like a two-way switch, where the switch controls the state of the light.

![rule](/en/getting_started/images/simple_rule.png?width=20pc&classes=shadow)

Now that you have configured your rules and devices, it's time to send this configuration to the server. [Next Chapter]({{% relref "getting_started/upload" %}})
