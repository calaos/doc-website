+++
title = 'What do I need?'
date = 2026-07-25T10:00:00Z
weight = 7
summary = "The hardware needed to run Calaos OS."
+++

## Shopping list

To install Calaos OS you need:

- **a PC-type computer** that will stay powered on permanently;
- **a USB stick** of at least 4 GB, used for the installation;
- **a network cable** to connect the computer to your router or switch.

And optionally:

- **a touchscreen**, if you want a wall interface in the house;
- a keyboard and a screen during the installation, if you would rather not configure the machine remotely.

## Which computer to choose

Calaos OS installs on a **64-bit PC** (x86_64 architecture). Both old and recent machines work: the installer handles modern computers (UEFI) as well as older ones (BIOS), and you have nothing to choose.

In practice, a **fanless mini PC** is the best choice for a permanent installation: it is silent, uses little power, and has no moving part that wears out. An old laptop or a spare desktop is perfectly fine to discover Calaos.

{{% notice tip %}}
To simply try Calaos OS, you need no dedicated hardware at all: **Live** mode boots the system from the USB stick without installing anything, and without touching the computer's disk. See [Installation]({{% relref "calaos_os/installation" %}}).
{{% /notice %}}

## The disk

Leave room to grow. Calaos OS takes little space at first, but three things grow over time:

- **the restore points**, those copies of the system state that let you roll back;
- **the images of the services** running on your server;
- **the history of your measurements**, if you record temperatures, power consumption, and so on.

The installer reserves roughly 2 to 2.5 GB at the start of the disk for booting and virtual memory, then uses all the rest. A small disk will work, but you will quickly run short. An SSD is preferable to a mechanical drive, for silence and speed.

## The network

**Connect the server with a cable** rather than over Wi-Fi. Your home automation must answer instantly and reliably, which a cable guarantees better.

Give your server a **fixed IP address**, so you always find it at the same place on your network. Otherwise its address may change across reboots and your applications will no longer find it. The procedure is on the [Network]({{% relref "calaos_os/configuration/network" %}}) page.

## The touchscreen

A touchscreen connected to the server displays **Calaos Home**, the local interface that shows your home and allows a few settings.

It is **optional**. Without a screen, you configure the server remotely from another computer, and you control your home from the mobile applications or the web app. This documentation always describes both ways.

For wall-mounted screens spread around the house, look at the [Remote UI screens]({{% relref "hardware/remote_ui" %}}) instead, which are designed for that purpose.

{{% notice note %}}
This page covers the **computer hardware** of your server. For everything about the electrical installation of the house — wiring, cabinet, PLC — see [Requirements]({{% relref "hardware/requirements" %}}) in the hardware chapter.
{{% /notice %}}

## Going further

The installer partitions the disk as follows:

| Machine | Partitions created |
|---|---|
| Modern (UEFI) | a 512 MB boot partition, a 2 GB swap partition, then the rest of the disk for the system |
| Older (BIOS) | a 2 GB swap partition, then the rest of the disk for the system |

The system partition is formatted with **Btrfs** with compression, and is labelled `calaos-os`. The details are described in [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).
