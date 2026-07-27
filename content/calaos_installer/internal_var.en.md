+++
title = 'Internal Variable'
date = 2024-03-06T20:12:36Z
weight = 30
summary = "Remembering a state that matches no physical device."
+++

## What it is for

An **internal variable** is an IO with no hardware behind it: it drives nothing and measures nothing, it simply holds a value.

This is what you start missing as soon as your automations get slightly clever. A few examples:

- **a mode** — "Holiday", "Night", "Nobody home" — that several rules consult;
- **a counter** — how many times someone passed a detector, how many times an alarm triggered;
- **a flag** — "watering already ran today", to stop a rule triggering twice;
- **a setting** you want to change from an interface without touching the configuration, such as a target temperature.

An internal variable is used in rules exactly like any other IO: as a condition and as an action.

## The three types

| Type | Holds | Typical use |
|---|---|---|
| **Boolean** | true or false | A mode being active or not, a flag |
| **Integer** | a number | A counter, a target value, a level |
| **String** | text | A message, a named state |

Choose the simplest one that fits. A "Holiday" mode is a boolean; if you need three exclusive modes, an integer or a string reads better than a collection of booleans you must remember to reset.

## Creating a variable

In the **Add** menu, choose **Internal variable**, then fill in:

- **the name**, as for any IO;
- **the type** among the three above;
- **the initial value**.

Three options complete the creation:

| Option | Effect |
|---|---|
| **Visible** | Shows the variable in the interfaces, to read or change it by hand |
| **Read / write** | Allows it to be changed from the interfaces, or reserves it for rules |
| **Save** | Keeps the value across a server restart |

## The "Save" option

This is the most important one, and the one whose absence shows at the worst moment.

Without it, the variable **goes back to its initial value on every restart** of the server. After an update or a power cut, your "Holiday" mode switches itself off, and the watering starts again while you are away.

{{% notice tip %}}
Ask yourself: "if the server restarts tonight, must this value be found unchanged?" If so, enable **Save**.

Conversely, a working variable — a counter reset every morning — is better off starting from zero.
{{% /notice %}}

## A complete example

To stop the automatic watering running twice on the same day:

1. Create a boolean variable **"Watering done"**, initial value *false*, without saving.
2. The watering rule gains a condition: only trigger if this variable is *false*.
3. In its actions, the rule sets the variable to *true*.
4. A time rule resets the variable to *false* every night.

The same principle — a variable remembering what has already happened — solves a large share of the automations that "keep triggering in a loop".

## Making a variable adjustable

A variable that is **visible** and **read/write** appears in the applications as a controllable element. It is the simplest way to offer a setting to the user: a target temperature, a mode, a number of minutes.

Your rules then read that variable instead of a value frozen in the configuration — and the user can adjust their installation without opening Calaos Installer.

## See also

- [Create rules]({{% relref "calaos_installer/rules" %}}) — to read and change your variables
- [LUA Scripting]({{% relref "calaos_installer/scripts" %}}) — to manipulate them in code
