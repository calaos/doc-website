+++
title = 'Rolling back'
date = 2024-03-06T20:14:42Z
weight = 40
summary = "Putting the server back into an earlier, working state."
+++

## What it is for

You are in the right place if:

- an update went wrong;
- the server no longer boots;
- something stopped working after a recent change.

Rolling back puts your server into the state it was in at an earlier date, the one you choose.

## How it works

Calaos OS automatically records the state of the system: before every software installation or update, and regularly over time. These recordings are called **restore points**, and they are dated.

You can ask the computer to restart on one of them. It then finds the system exactly as it was on that day.

{{% notice warning %}}
**Rolling back also restores your Calaos configuration.**

Your inputs/outputs, rules and scenarios go back to the chosen date as well. Everything you created since will be lost, including the configuration history recorded by the server.

An example: you update on Monday, you create rules on Tuesday, you roll back on Wednesday to Monday — Tuesday's work disappears.

**So retrieve your configuration before confirming.** Step 3 below gives you all the time you need.
{{% /notice %}}

## 1. Choose a date at boot

Restart the computer. Just after power-on, a menu is displayed for five seconds.

Use the arrow keys to enter the submenu listing the restore points, then choose the date you want — usually the most recent one before the problem. Confirm with Enter.

{{% notice tip %}}
If the menu disappears too quickly, restart and press the up arrow several times as soon as the machine powers on: this stops the countdown and gives you all the time you need to choose.
{{% /notice %}}

The system then boots on the chosen restore point.

## 2. Nothing is final yet

At this stage the system runs in **trial mode**: it shows you what your server would look like if you went back to that date, but nothing has been written yet.

{{% notice info %}}
**If you change your mind, simply restart the computer.** It will boot the current system again, as if nothing had happened.
{{% /notice %}}

This is the moment to check that this restore point is the right one: does your home automation respond correctly? Has the problem gone? If not, restart and choose an earlier date.

## 3. Retrieve your configuration

Since rolling back will also restore your Calaos configuration, set it aside now.

The simplest way is to bring it back to your computer with **Calaos Installer**: menu **Calaos Server**, option **Load project from Calaos Server**, then save the project locally. The details are described in [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

## 4. Confirm

### With a touchscreen

When you have booted on a restore point, the **Calaos Home** interface automatically displays a window explaining the situation and offering a button to confirm the rollback. You have nothing to look for: click that button.

### Without a touchscreen

The window above only appears if a touchscreen is connected. Otherwise, connect to the server over SSH — this works normally, even when booted on a restore point:

```sh
ssh root@YOUR_SERVER_ADDRESS
```

then run:

```sh
calaos_rollback
```

{{% notice note %}}
If the command answers `Not booted from a readonly btrfs snapshot`, the computer did not boot on a restore point but on the usual system. Start again from step 1.
{{% /notice %}}

## 5. Restart

Restart one last time:

```sh
reboot
```

Your server boots into the restored state. You are done.

## What is not restored

**The system logs are kept.** They are not part of the restore points, so you can still look afterwards at what happened at the time of the incident. See [Logs]({{% relref "calaos_os/configuration/logs" %}}).

## If nothing works

If no restore point gives you a working server back, a full reinstall remains: make a new USB stick, reinstall Calaos OS following [Installation]({{% relref "calaos_os/installation" %}}), then send your configuration back from Calaos Installer.

## Going further

Restore points rely on Btrfs snapshots, managed by snapper. To list them from a terminal:

```sh
snapper list
```

The details of the mechanism, of the automatic retention and of what is covered or not are described in [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).
