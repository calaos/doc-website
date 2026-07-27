+++
title = "Calaos Installer"
date = 2024-03-06T19:56:48Z
weight = 30
chapter = true
+++

### The configuration

# Calaos Installer

Calaos Installer is the software with which you describe your installation: the rooms of the house, the devices in them, and the rules that make them react.

It runs on **your everyday computer**, not on the server. You prepare your project there, then send it to the Calaos server, which applies it.

## How it fits together

```text
Your computer                             The Calaos server
┌────────────────────┐                    ┌────────────────────┐
│  Calaos Installer  │  ── upload ──▶     │   calaos_server    │
│  your project      │  ◀── read ────     │   drives the home  │
└────────────────────┘                    └────────────────────┘
```

You always work on a local copy of the project. Nothing changes at home until you **send** the configuration to the server, which leaves you free to experiment safely.

The server, for its part, keeps a dated copy of every configuration it receives — see [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

## The order of things

The configuration is built in a natural order, each step relying on the previous one:

1. **[Create rooms]({{% relref "calaos_installer/rooms" %}})** — the structure of your home.
2. **[Create IOs]({{% relref "calaos_installer/io" %}})** — the devices: lights, shutters, switches, sensors.
3. **[Create rules]({{% relref "calaos_installer/rules" %}})** — what should happen, and when.

The other pages of this chapter cover more specific needs: [internal variables]({{% relref "calaos_installer/internal_var" %}}) to remember a state, [scenarios]({{% relref "calaos_installer/scenario" %}}) to chain actions, [time scheduling]({{% relref "calaos_installer/time" %}}) to trigger on the clock, and [Lua scripts]({{% relref "calaos_installer/scripts" %}}) when rules are no longer enough.

{{% notice tip %}}
If you are new to Calaos, do not start with this chapter: follow [Getting started]({{% relref "getting_started" %}}) first, which walks you through creating a room, a light, a switch and a first rule. You will come back here to go deeper.
{{% /notice %}}

## Connecting to the server

Two operations link Calaos Installer to your server, in the **Calaos Server** menu:

- **Send the project** — your configuration goes to the server, which restarts its services to take it into account;
- **Load the project** — you fetch the configuration currently running on the server.

The second is also the simplest way to **back up** your installation: see [Upload the project]({{% relref "getting_started/upload" %}}).

These operations ask for the server address and the login credentials, whose default values are `user` / `pass` — to be changed, see [Changing the passwords]({{% relref "calaos_os/security" %}}).

## Beyond configuration

Calaos Installer is also used for a few tasks that go beyond the project itself:

- **creating the Calaos OS installation USB stick** — see [Installation]({{% relref "calaos_os/installation" %}});
- **flashing and configuring Remote UI wall screens** — see [Remote UI]({{% relref "hardware/remote_ui" %}});
- **programming the Wago PLC** — see [Wago Configuration]({{% relref "calaos_installer/wago" %}}).

{{% children description="true" %}}
