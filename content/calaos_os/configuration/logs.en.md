+++
title = 'Logs'
date = 2024-03-06T20:15:22Z
weight = 30
summary = "Reading the system logs to understand a problem."
+++

## What logs are for

The system permanently records what it does in **logs**. When something does not work, that is where the explanation is: which service failed, when, and why.

Everything is gathered in one place, whichever service is involved: a single command, `journalctl`, lets you read it all.

You do not need to understand everything. Spotting the moment of the problem and copying the few matching lines is more than enough to ask for help.

This page is used over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})).

## Watch what happens live

```sh
journalctl -f
```

Lines appear as the system writes them. This is very handy to reproduce a problem: run this command, trigger the malfunction, and watch what appears.

Press `Ctrl` + `C` to stop the display.

## See the logs of one service

```sh
journalctl -u SERVICE_NAME
```

To find a service name, see the [Services]({{% relref "calaos_os/configuration/services" %}}) page.

Add `-f` to follow that service live:

```sh
journalctl -u SERVICE_NAME -f
```

## Limit to a period

Logs are large. To see only what happened recently:

```sh
journalctl --since "1 hour ago"
journalctl --since today
journalctl --since "2026-07-25 14:00"
```

And to keep only errors:

```sh
journalctl -p err --since today
```

## Boot logs

To see what happened during the last boot:

```sh
journalctl -b
```

And during the previous boot, useful when the machine restarted on its own:

```sh
journalctl -b -1
```

## Asking for help

To send logs to the forum or to a bug report, save them into a file:

```sh
journalctl -b > /tmp/journal.txt
```

Then fetch that file onto your computer:

```sh
scp root@YOUR_SERVER_ADDRESS:/tmp/journal.txt .
```

{{% notice tip %}}
Always state **the approximate time** at which the problem happened: that is what makes it possible to find the right lines in a log containing thousands of them.
{{% /notice %}}

{{% notice warning %}}
Read what you publish: logs can contain addresses, device names, sometimes credentials. Remove anything you do not want to make public.
{{% /notice %}}

## Logs of the additional services

Nothing special to remember: services running in containers write to the same journal as the rest of the system. The command shown above works for them identically:

```sh
journalctl -u zigbee2mqtt
```

See [Services and modules]({{% relref "calaos_os/containers" %}}).

## Going further

Logs are stored in `/var/log`, which is **excluded from restore points**. Concretely: after a rollback, the logs of the incident are still there, and you can still understand what happened. See [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).

To know how much space the logs take, and reduce it if needed:

```sh
journalctl --disk-usage
journalctl --vacuum-time=30d
```
