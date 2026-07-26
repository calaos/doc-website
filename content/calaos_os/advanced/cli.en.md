+++
title = "The calaos-os tool"
date = 2026-07-25T10:00:00Z
weight = 10
summary = "Command reference for the calaos-os tool."
+++

## Purpose

`calaos-os` is the system administration tool. It handles updates and network configuration, and usefully replaces the equivalent Debian commands, since it knows the specifics of Calaos OS.

It is used from a terminal on the server, or remotely over SSH — see [SSH access]({{% relref "calaos_os/advanced/shell" %}}).

## Command reference

| Command | Effect |
|---|---|
| `calaos-os list` | Lists installed images and packages, and available updates |
| `calaos-os check-update` | Checks whether updates are available |
| `calaos-os upgrade` | Updates every component |
| `calaos-os upgrade PACKAGE` | Updates a single component |
| `calaos-os network list` | Lists the network interfaces |
| `calaos-os network configure dhcp INTERFACE` | Configures an interface with an automatic address |
| `calaos-os network configure static INTERFACE IPV4 NETMASK [GATEWAY] [DNS...]` | Configures an interface with a fixed address |

## Updates

A complete update cycle looks like this:

```sh
# See the current state of the system
calaos-os list

# Check what is available, without changing anything
calaos-os check-update

# Update everything
calaos-os upgrade
```

To update a single component, for instance to try a new version of a service without touching the rest:

```sh
calaos-os upgrade PACKAGE_NAME
```

A restore point is created automatically before every operation, whichever command you use. See [Rolling back]({{% relref "calaos_os/rollback" %}}).

{{% notice info %}}
As Calaos OS is based on Debian, `apt` also works and acts on the same package repository. `calaos-os upgrade` and `apt full-upgrade` produce the same result.
{{% /notice %}}

The complete step-by-step procedure is on the [Update]({{% relref "calaos_os/update" %}}) page.

## Network configuration

Always start by finding the interface name:

```sh
calaos-os network list
```

Then configure it, with an automatic address:

```sh
calaos-os network configure dhcp eth0
```

or with a fixed address:

```sh
calaos-os network configure static eth0 192.168.1.50 255.255.255.0 192.168.1.1 192.168.1.1
```

The arguments are, in order: the interface, the IPv4 address, the network mask, the gateway, then one or more DNS servers separated by spaces. The gateway and DNS are optional, but without them the server will have no Internet access.

{{% notice warning %}}
If you are connected over SSH and change the interface you are connected through, **your session will be dropped**. Reconnect to the new address.
{{% /notice %}}

Detailed explanations are on the [Network]({{% relref "calaos_os/configuration/network" %}}) page.

## Not to be confused

Two tools have similar names and are unrelated:

- **`calaos-os`** — with a hyphen — administers **the system**: updates, network;
- **`calaos_config`** — with an underscore — manages the **Calaos server settings**: credentials, notifications. See its [own page]({{% relref "calaos_os/configuration/calaos_config" %}}).
