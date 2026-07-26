+++
title = 'Update'
date = 2024-03-06T20:14:28Z
weight = 20
summary = "Updating the system and the services, safely."
+++

## Updating is safe

Before every software installation, Calaos OS automatically records the complete state of the system. If an update goes wrong, you can restart on the previous state and get a working server back.

You have nothing to do for these restore points to exist: they are created on their own. See [Rolling back]({{% relref "calaos_os/rollback" %}}).

Calaos OS updates are **incremental**: only the components that changed are downloaded and replaced. You never need to make a new USB stick or reinstall.

{{% notice info %}}
**Your configuration is not affected by an update.** Your inputs/outputs, rules, scenarios and measurement history are kept unchanged.
{{% /notice %}}

## Before you start

Back up your configuration: it is quick and will save you trouble one day. The procedure is on [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

Also pick a moment when an interruption will bother nobody. During the update your home automation may stop responding for a few moments, and a restart is sometimes needed at the end.

## Connecting to the server

The update is started from the server. The simplest way is to connect remotely from your everyday computer.

Open a terminal — the **Terminal** application on macOS and Linux, **PowerShell** or **Windows Terminal** on Windows — and type:

```sh
ssh root@YOUR_SERVER_ADDRESS
```

replacing `YOUR_SERVER_ADDRESS` with the IP address of your server. The password is the one you set in [Changing the passwords]({{% relref "calaos_os/security" %}}).

{{% notice tip %}}
You can also plug a keyboard and a screen directly into the server and type the commands there. It is less convenient, but works exactly the same.
{{% /notice %}}

## 1. See what is available

Type:

```sh
calaos-os check-update
```

This command **changes nothing**: it only checks whether new versions exist and shows them to you. You can run it as often as you like, with no risk at all.

If nothing is available, your server is up to date and there is nothing else to do.

## 2. Start the update

Type:

```sh
calaos-os upgrade
```

The download starts, then the installation. Many lines scroll past: that is normal, it is the detail of what is being installed. You do not have to read them.

**Do not close the window and do not cut the power** until the command has finished. Depending on the number of components to update and the speed of your connection, allow from a few minutes to about twenty.

It is done when the scrolling stops and the terminal hands control back to you, showing a prompt waiting for your next command.

## 3. Check that everything works

Go back to using your installation normally: switch on a light from the mobile application, check that a scenario triggers. If everything responds as before, the update succeeded.

If the server needs a restart, restart it:

```sh
reboot
```

## If something goes wrong

Do not reinstall anything and do not panic: this is exactly the situation restore points exist for. Go to [Rolling back]({{% relref "calaos_os/rollback" %}}), which brings you back to the state before the update in a few minutes.

If the server still boots but one particular service no longer works, the [Something went wrong]({{% relref "calaos_os/troubleshooting" %}}) page will help you identify which one.

## Going further

**See what is installed**, including versions:

```sh
calaos-os list
```

**Update a single component** rather than everything:

```sh
calaos-os upgrade PACKAGE_NAME
```

**Use the Debian tools.** As Calaos OS is based on Debian, the `apt` tool also works and acts on the same package repository:

```sh
apt update
apt full-upgrade
```

The result is the same, and restore points are created either way.

All the commands of the tool are detailed in [The calaos-os tool]({{% relref "calaos_os/advanced/cli" %}}).
