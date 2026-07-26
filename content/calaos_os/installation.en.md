+++
title = 'Installation'
date = 2024-03-06T20:13:46Z
weight = 10
summary = "Prepare the USB stick, try it out, then install on the disk."
+++

## What you are going to do

The installation happens in three steps:

1. you prepare a USB stick containing Calaos OS;
2. you boot the computer from that stick, which lets you try the system **without installing anything**;
3. when you are happy with it, you install onto the computer's disk.

Most of the time is spent waiting. Allow plenty of it: the first boot from the stick alone can take an hour if your USB stick is slow (see step 4).

## 1. Prepare the USB stick

The stick is prepared from **Calaos Installer**, the software you install on your everyday computer.

In the **Project management** menu, choose **Create an image**. Select the latest stable version of Calaos OS from the list, then click **Continue**: the image is downloaded from the Calaos servers, which can take a few minutes depending on your connection.

Then insert your USB stick. It appears in the window: select it and click **Continue** so the image is copied onto it.

{{% notice warning %}}
The USB stick is **completely erased** during this operation. Make sure you selected the stick, and not an external drive holding your documents.
{{% /notice %}}

The detailed procedure with screenshots is described in [Installation]({{% relref "getting_started/installation" %}}), in the Getting started chapter.

## 2. Boot the computer from the stick

Plug the stick into the computer that will become your server, then switch it on.

Most computers boot from their internal disk by default. To ask them to boot from the stick, you must press a key as soon as the machine powers on, before the system logo appears. That key depends on the brand: most often **F12**, sometimes **F11**, **F9**, **F8** or **Esc**. A menu then appears, in which you choose the USB stick.

{{% notice tip %}}
If the stick does not appear in the list, shut the computer down completely, check that the stick is properly plugged in — preferably into a USB port at the back of the machine — and try again. If the problem persists, see the [Something went wrong]({{% relref "calaos_os/troubleshooting" %}}) page.
{{% /notice %}}

## 3. Choose between trying and installing

Once booted from the stick, a menu offers two options:

- **start Calaos OS in Live mode**, that is, run it directly from the USB stick;
- **install Calaos OS** onto the computer's disk.

You have five seconds to choose with the arrow keys, then Enter. If you do nothing, Live mode starts.

## 4. The first boot is slow

{{% notice warning %}}
**On the very first boot from the stick, Calaos OS has to unpack and install all its services. This takes a long time.**

A welcome screen appears with the message *"Calaos-OS is currently initializing. Please wait..."*. Allow **several minutes** on a fast stick, and **up to 30 to 60 minutes on a slow one**.

The computer has not crashed. Do not switch it off and do not restart it: let it finish, even if nothing seems to happen for a long while.
{{% /notice %}}

This only happens **once**. Later boots from the stick are quick.

{{% notice tip %}}
The duration depends almost entirely on the speed of your USB stick. If you need to buy one, get a branded USB 3.0 stick: you will save most of this waiting time, both here and when copying the image.
{{% /notice %}}

## 5. Try without installing anything

**Live** mode runs a complete Calaos OS system from the USB stick.

{{% notice info %}}
**The computer's disk is not touched.** You can explore, configure, test your hardware: nothing is written to the internal disk. If you are not happy with the result, switch the machine off and remove the stick — no trace will remain.
{{% /notice %}}

This is the ideal moment to check that your hardware works: that the network card is recognised, that the screen displays correctly, that your USB devices are detected.

You can even configure your home automation already — create your inputs/outputs and your rules — because **that configuration is carried over when you install onto the disk** (see step 7).

## 6. Install onto the disk

There are two ways to start the installation, depending on your equipment.

### From the Calaos Home interface

If a touchscreen is connected to the machine and you booted in Live mode, the **Calaos Home** interface offers to install the system onto the disk. This is the simplest method: everything happens on screen, without a keyboard.

### From the boot menu

Otherwise, restart the computer from the USB stick and this time choose the **Installation** entry in the menu from step 3.

A confirmation screen asks whether you want to install Calaos OS. If you answer no, the machine simply carries on booting in Live mode.

If you answer yes, the list of disks in the machine is displayed, each with its model and capacity. Select the one to install onto, then confirm. Navigation uses the arrow keys, the space bar to tick, and Enter to confirm.

{{% notice warning %}}
**The disk you choose is completely erased.** Everything it contains is permanently lost. Check the model and capacity displayed before confirming, especially if the machine holds several disks.
{{% /notice %}}

## 7. Wait

The installation prepares the disk, copies the system onto it and sets up booting. It also records the first restore point of your server.

This step takes several minutes. Do not cut the power. When it is finished, the computer restarts automatically: remove the USB stick so that it boots from its disk this time.

{{% notice tip %}}
**Your Live mode configuration is kept.** Everything you configured during the trial — Calaos settings, inputs/outputs, rules — is found unchanged on the installed system. You are not starting from scratch.
{{% /notice %}}

## 8. First boot

The computer now boots from its disk.

{{% notice warning %}}
**This first boot is slow too.** The system reinstalls its services onto the disk, exactly as it did on the stick. The same waiting screen appears.

It is usually quicker than on the stick, as the disk is faster, but still allow several minutes. Again, let the machine finish. And again, this only happens **once**.
{{% /notice %}}

Once that step is done, the Calaos server starts automatically, along with the additional services.

If a screen is connected, the Calaos Home interface is displayed. Otherwise the server is reachable over the network: from Calaos Installer, on your everyday computer, you will find it and send it your configuration — see [Upload the project]({{% relref "getting_started/upload" %}}).

{{% notice warning %}}
**One thing remains before using your server day to day: change the default passwords.** They are identical on every Calaos OS installation and published in this documentation. Go to [Changing the passwords]({{% relref "calaos_os/security" %}}); it only takes a few minutes.
{{% /notice %}}

## Going further

The installer does more than copy files. It:

- partitions the disk and formats the main partition with **Btrfs** with compression;
- creates a series of subvolumes that separate the system from variable data (logs, caches);
- copies the Live system as it runs, **including your configuration**;
- exports the container images into a local cache, which lets the server boot for the first time **without Internet access**;
- installs the bootloader and creates restore point number 1, dated from the installation.

The details of the disk layout are described in [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).
