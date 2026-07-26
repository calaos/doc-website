+++
title = "What is Calaos OS?"
date = 2026-07-25T10:00:00Z
weight = 5
summary = "What Calaos OS is and what it gives you."
+++

## In a nutshell

Calaos OS is a complete, ready-to-use operating system that you install on a computer dedicated to your home automation. That computer becomes your **Calaos server**: it drives your lights, shutters and heating, and answers the mobile applications.

You do not need to install an operating system and then add the Calaos software on top: everything is already assembled and preconfigured. You copy the system onto a USB stick, boot the computer from it, and install.

## What it means for you

**Everything is already there.** The Calaos server and the additional services (MQTT, Zigbee, measurement history, graphs, DMX lighting…) are installed and configured to work together. You have nothing to assemble yourself.

**Updates can be undone.** Before every software installation, Calaos OS automatically records the state of the system. If an update goes wrong, you can restart on the previous state. This is explained in [Rolling back]({{% relref "calaos_os/rollback" %}}).

**Your configuration lives in one place.** All your settings — those of Calaos and those of the services — live in a single folder, which makes backups simple. See [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

## What you need

A computer that will stay powered on permanently, a USB stick, and a way to connect it to your network. The details are on the [What do I need?]({{% relref "calaos_os/requirements" %}}) page.

A touchscreen is **optional**. With a screen you get the local Calaos Home interface, which displays your home and allows a few settings. Without a screen, everything is configured remotely from another computer.

{{% notice note %}}
This documentation only covers **Calaos v4**. Previous versions (v1, v2, v3) are not covered.
{{% /notice %}}

## Where to start

If you are new to Calaos, follow the [Getting started]({{% relref "getting_started" %}}) chapter: it guides you from installation to your first automation rule.

If you want to install the server right now, go straight to [Installation]({{% relref "calaos_os/installation" %}}).

## Going further

Calaos OS is built on **Debian**. The application components (Calaos server, MQTT, Zigbee, databases…) run in **Podman containers**, isolated from each other, which allows them to be updated independently.

The disk is formatted with **Btrfs**, a filesystem able to photograph the state of the system at a given moment for almost no disk space. This is what makes rollbacks possible. The details are described in [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).

The complete software architecture is presented in the [Calaos software stack]({{% relref "dev" %}}) chapter.
