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

Calaos OS automatically records the state of the system: before every software installation or update, and regularly over time. These recordings are called **restore points**, and they are dated. The web app calls them **snapshots**: it is the same thing.

You can ask the computer to restart on one of them. It then finds the system exactly as it was on that day.

{{% notice warning %}}
**Rolling back also restores your Calaos configuration.**

Your inputs/outputs, rules and scenarios go back to the chosen date as well. Everything you created since will be lost, including the configuration history recorded by the server.

An example: you update on Monday, you create rules on Tuesday, you roll back on Wednesday to Monday — Tuesday's work disappears.

**So retrieve your configuration before confirming.** Step 3 below gives you all the time you need.
{{% /notice %}}

## 1. Choose a restore point

There are two ways to restart on a restore point. They lead to the same result.

### From the web app

You need a working server and the [admin password]({{% relref "calaos_os/admin" %}}).

1. Open the web app over `https://`, then the **Admin** tab, and unlock it.
2. Open the **Snapshots** tile. The restore points are listed, newest first, with their date, their type and a description. Two badges help you: **Current** (the one the system is running from) and **Default** (the one a normal start uses).
3. Click **Reboot on this** on the line you want, usually the most recent one before the problem, then confirm with **Reboot**.

The box restarts on that restore point by itself, once only. It stays offline for 1 to 3 minutes, and the page reconnects on its own. Do not send the request a second time if the page says it is waiting for the box.

A restore point shown as **Not bootable** cannot be started this way. This is notably the case of the oldest ones, recorded before the system was able to start on a restore point in trial mode, which the web app refuses as too old. Use the boot menu (below) for those.

### From the boot menu

Restart the computer. Just after power-on, a menu is displayed for five seconds.

Use the arrow keys to enter the submenu listing the restore points, then choose the date you want — usually the most recent one before the problem. Confirm with Enter.

{{% notice tip %}}
If the menu disappears too quickly, restart and press the up arrow several times as soon as the machine powers on: this stops the countdown and gives you all the time you need to choose.
{{% /notice %}}

The system then boots on the chosen restore point.

## 2. Nothing is final yet

At this stage the system runs in **trial mode** (also called **rescue mode**): it shows you what your server would look like if you went back to that date, but nothing is kept on the disk. Anything you change while in this mode is lost at the next restart.

The web app shows a **Rescue mode** banner on all its pages, with the number of the restore point. The touchscreen shows the **Recovery System** window.

{{% notice info %}}
**If you change your mind, simply restart the computer.** It will boot the current system again, as if nothing had happened. In the web app, **Admin**, **Snapshots**, then **Return to normal system** does the same.
{{% /notice %}}

While in trial mode, **updates are refused**: neither the web app nor the terminal can install anything until you leave this mode.

This is the moment to check that this restore point is the right one: does your home automation respond correctly? Has the problem gone? If not, restart and choose an earlier date.

## 3. Retrieve your configuration

Since rolling back will also restore your Calaos configuration, set it aside now.

The simplest way is to bring it back to your computer with **Calaos Installer**: menu **Calaos Server**, option **Load project from Calaos Server**, then save the project locally. The details are described in [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

## 4. Confirm

### From the web app

In trial mode, the **Rescue mode** banner explains the situation. Its information window and the **Admin**, **Snapshots** panel offer **Roll back: make permanent**. The admin password is asked for if the Admin tab is locked. Tick **I understand this is permanent**, then click **Roll back**.

The box restarts by itself. It stays offline for 1 to 3 minutes, and the page reconnects on its own.

### With a touchscreen

When you have booted on a restore point, the **Calaos Home** interface offers a **Recovery System** window explaining the situation, with a **Rollback** button: click it. The window is also reachable from the **Configuration** menu of the control panel. **Close** hides the window without changing anything.

The box restarts by itself.

If a message tells you that an update or a restart is already in progress, wait a few minutes and try again. If the connection is lost, the box is probably restarting: wait for it to reconnect before trying again.

### Over SSH

If you have neither a touchscreen nor the web app, connect to the server over SSH — this works normally, even when booted on a restore point:

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

With the web app or the touchscreen, the box restarts by itself two seconds after the confirmation: there is nothing to do. Over SSH, restart yourself one last time:

```sh
reboot
```

Your server boots into the restored state. You are done.

## Limits and precautions

- **Rolling back is permanent.** Your configuration since that date is gone: see the warning above.
- **One operation at a time.** While an update or a rollback runs, the web app and the touchscreen refuse a new one. Wait for the end, which can take a few minutes.
- **A failed rollback leaves the box as it was.** If something fails before the end, the system puts the previous state back and the box restarts normally on it; the message displayed names the cause. You can try again once it is fixed.
- **If a message says _do not reboot_**, stop there and do not restart: the rollback could not be cleanly undone and the fix is manual, from a shell on the box. Keep the message displayed.
- **Without keyboard or screen, a restore point that crashes can loop.** On a box that starts in legacy BIOS mode (or without an EFI partition), a restore point started once from the web app that crashes very early can be started again at every restart. GRUB then waits 3 seconds without showing its menu: pressing **Escape** during that delay displays the normal menu, where you choose the usual system. A box with neither keyboard nor screen then needs an external USB stick.
- **The web app lists the 200 most recent restore points.**

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
