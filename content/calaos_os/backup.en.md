+++
title = 'Backing up your configuration'
date = 2026-07-25T10:00:00Z
weight = 45
summary = "Keeping your configuration safe, and restoring it when needed."
+++

## What needs backing up

Your server's whole configuration lives in a single folder: `/mnt/calaos`. It contains:

- your Calaos settings: inputs/outputs, rules, scenarios, rooms;
- the history of your measurements, if you record temperatures or power consumption;
- the settings of the additional services: MQTT, Zigbee, graphs, DMX lighting, and so on.

It is also the folder that is **never touched by an update**.

## The simplest method: Calaos Installer

Calaos Installer can read your whole configuration back from the server and save it onto your computer. No command line is needed.

In Calaos Installer, menu **Calaos Server**, choose **Load project from Calaos Server**. The configuration files are downloaded and the project opens. Then save it onto your computer, somewhere you will remember.

You get a complete copy of your configuration, **stored somewhere other than the server**: which is exactly the point of a backup.

The procedure is described in detail in [Upload the project]({{% relref "getting_started/upload" %}}).

## When to make a backup

- **Before an update**, as a precaution.
- **Before confirming a rollback**, because it also restores your configuration — see [Rolling back]({{% relref "calaos_os/rollback" %}}).
- **After a long configuration session**, so you do not risk redoing two hours of work.

## Where to store it

**Somewhere other than the server.** A backup that stays on the machine is useless the day that machine's disk fails. Save the project onto your computer, and ideally copy it onto an external drive or online storage as well.

## The server's automatic history

Every time you send a new configuration from Calaos Installer, the Calaos server files **a dated copy** of the previous configuration, in a folder organised by year and month:

```text
/mnt/calaos/config/backups/2026/07/01-07-2026_16-56-00/
├── io.xml
├── local_config.xml
└── rules.xml
```

Every upload therefore leaves a trace, which lets you go back to an earlier version of your configuration.

{{% notice warning %}}
**This history is not a backup.** It sits on the server itself, inside `/mnt/calaos`. So it protects you neither from a disk failure nor from a rollback — which will restore it like the rest and make the recent versions disappear.

Think of it as a handy safety net, not as your backup.
{{% /notice %}}

## Restore point or backup?

Both exist, and they do not protect against the same things:

| | Protects against | Stored |
|---|---|---|
| **Restore point** | a failed update, a broken system | on the server |
| **Backup** | a disk failure, theft, a configuration mistake | elsewhere |

A restore point does not replace a backup: if the server's disk dies, both disappear together.

## Restoring

After reinstalling Calaos OS, open your saved project in Calaos Installer, then send it back to the server with **Send project to Calaos Server**. Your home automation starts again as before.

## Going further

To also back up your measurement history and the settings of the additional services, copy the whole `/mnt/calaos` folder. From your computer:

```sh
scp -r root@YOUR_SERVER_ADDRESS:/mnt/calaos ./calaos-backup
```

This copy is larger and slower, but it is exhaustive.
