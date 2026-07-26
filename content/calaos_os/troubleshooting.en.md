+++
title = 'Something went wrong'
date = 2026-07-25T10:00:00Z
weight = 70
summary = "Common failures, and what to do in each case."
+++

## How to use this page

Find the symptom that most closely matches what you see, and follow the checks in the order given. They go from the simplest to the most involved: do not skip the first ones, they are often the right ones.

---

## The computer does not boot from the USB stick

**First check** that the stick is properly plugged in, preferably into a USB port at the back of the machine, and that the computer is fully switched off before you turn it on again.

**Call up the boot menu.** As soon as the machine powers on, press the boot menu key several times: most often **F12**, sometimes **F11**, **F9**, **F8** or **Esc**. If you do not know which one, try them one after the other, restarting each time.

**Disable Secure Boot.** Some computers refuse to boot anything other than their original system. Enter the machine settings — **Del** or **F2** key at power-on — and look for an option called *Secure Boot*, to be disabled.

**Make the stick again.** A badly copied stick will not boot. Redo the preparation described in [Installation]({{% relref "calaos_os/installation" %}}), and try another stick if you can.

---

## The first boot seems stuck

You see the Calaos OS welcome screen and the message *"Calaos-OS is currently initializing. Please wait..."*, and nothing has happened for a long time.

**This is normal, and you must not switch the machine off.** On the very first boot, Calaos OS unpacks and installs all its services. On a slow USB stick, this step can last **30 to 60 minutes**.

Let it run. It only happens once: later boots are quick.

If you did switch the machine off along the way, simply turn it back on — installing the services will resume.

{{% notice tip %}}
For next time: the duration depends almost entirely on the speed of your USB stick. A branded USB 3.0 stick brings this wait down to a few minutes. See [What do I need?]({{% relref "calaos_os/requirements" %}}).
{{% /notice %}}

---

## The server no longer boots since an update

This is exactly the situation restore points exist for: your previous system is still there.

Go to [Rolling back]({{% relref "calaos_os/rollback" %}}) and follow the procedure. It takes a few minutes and requires no particular skill.

---

## I can no longer find my server on the network

**Is the server switched on?** Check the machine's indicator lights, and the network port light on the router or switch.

**Has its address changed?** This is the most frequent cause, especially after a power cut. Look at the list of connected devices in your router's interface: your server should be there, with its new address.

**To stop this happening again**, give it a fixed address: see [Network]({{% relref "calaos_os/configuration/network" %}}).

**If you have access to the machine**, a screen and a keyboard connected to it let you check its network configuration with:

```sh
calaos-os network list
```

---

## My applications no longer connect to the server

**Did you change the credentials recently?** After a change, every application must be reconfigured with the new ones: Calaos Installer, the mobile applications, the web app. See [Changing the passwords]({{% relref "calaos_os/security" %}}).

**Does the server answer?** Test from a computer on the same network. If the server does not answer at all, see the previous symptom.

**Only from outside?** The problem probably comes from your remote access: see [Dynamic DNS]({{% relref "calaos_os/configuration/calaos_ddns" %}}).

---

## The touchscreen stays black or does not respond

See the [Touchscreen]({{% relref "calaos_os/configuration/touchscreen" %}}) page, which details the checks to make.

Remember that **the server works even without a screen**: if your home automation responds from the mobile applications, the problem is purely a display one, and your installation is not at risk.

---

## Part of my installation no longer responds

If only some devices stop responding — your Zigbee bulbs, your temperature readings — the matching service is probably at fault, not Calaos as a whole.

Connect over SSH and look at what failed:

```sh
systemctl --failed
```

This command covers **every** service, including those for Zigbee devices, MQTT or the history.

Then check the logs of the service concerned, as described in [Logs]({{% relref "calaos_os/configuration/logs" %}}).

Restarting the service is often enough: see [Services]({{% relref "calaos_os/configuration/services" %}}).

---

## The disk is full

Three things take space and grow over time: restore points, the history of your measurements, and logs.

**See what is using the disk:**

```sh
df -h /
```

**Shrink the logs**, if they are the culprit:

```sh
journalctl --disk-usage
journalctl --vacuum-time=30d
```

**Restore points** are normally cleaned up automatically: Calaos OS only keeps a limited number and deletes the oldest on its own. If the disk fills up anyway, see [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).

{{% notice warning %}}
Do not delete files at random to make space, particularly inside `/mnt/calaos`: that is where your whole configuration lives.
{{% /notice %}}

---

## The time is wrong, my rules trigger at the wrong moment

See [Date & Time]({{% relref "calaos_os/configuration/date" %}}). A shifted clock also causes Remote UI screens to refuse connections.

---

## Asking for help

If none of the above solves your problem, the Calaos forum and the [GitHub repository](https://github.com/calaos/) are there for that.

To get a useful answer straight away, state:

- **what you observe**, and since when;
- **what changed just before**: an update, a new device, a power cut;
- **the approximate time** of the problem;
- **the matching logs**, retrieved as explained in [Logs]({{% relref "calaos_os/configuration/logs" %}}).

{{% notice tip %}}
Before reinstalling everything, ask: a reinstall wastes time and often erases the traces that would have explained the failure. In most cases a rollback or restarting a service is enough.
{{% /notice %}}
