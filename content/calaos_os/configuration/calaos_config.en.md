+++
title = 'calaos_config'
date = 2024-03-06T20:15:28Z
weight = 40
summary = "The tool that reads and changes the Calaos server settings."
+++

## What this tool is for

`calaos_config` is the tool that lets you view and change the Calaos server settings from a terminal. It is the one used by most pages of this chapter.

It works on the server's local configuration, stored in the `/mnt/calaos/config` folder.

{{% notice info %}}
This tool only concerns **the server settings**: credentials, notifications, operating options. Your inputs/outputs, rules and scenarios are managed from **Calaos Installer**, not here.
{{% /notice %}}

## Usage

Connect to the server over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})), then use one of the four following commands.

| Command | Effect |
|---|---|
| `calaos_config list` | Shows every setting and its value |
| `calaos_config get KEY` | Shows the value of one setting |
| `calaos_config set KEY VALUE` | Changes a setting |
| `calaos_config del KEY` | Deletes a setting |

Each setting is identified by a **key**, that is, a short name without spaces.

## See every setting

The most useful command to begin with:

```sh
calaos_config list
```

It shows all the current settings of your server, one per line, in the form `key: value`. It is the best way to discover what is configured on your installation.

This command **changes nothing**, you can run it without worry.

## Read one setting

```sh
calaos_config get cn_user
```

shows the value of the `cn_user` setting, that is, the user name used to connect to the server.

## Change a setting

```sh
calaos_config set cn_user my_user
```

The change is saved immediately. Some settings are only taken into account after the Calaos server restarts.

## Delete a setting

```sh
calaos_config del SETTING_NAME
```

The setting disappears from the configuration, and the server goes back to its default behaviour for that option.

## A few useful keys

| Key | Purpose | Related page |
|---|---|---|
| `cn_user` | User name used to connect to the server | [Changing the passwords]({{% relref "calaos_os/security" %}}) |
| `cn_pass` | Password used to connect to the server | [Changing the passwords]({{% relref "calaos_os/security" %}}) |
| `hwid` | Hardware identifier of the machine, read-only | — |

The other available keys depend on the features you use; `calaos_config list` shows you the ones defined on your installation.

{{% notice warning %}}
**Do not edit the configuration files by hand** while the server is running. Your changes may be overwritten, and a malformed file will prevent the server from starting. Always go through `calaos_config`.
{{% /notice %}}

## Going further

Settings are stored in the `local_config.xml` file, in `/mnt/calaos/config`. That folder also holds the history of the configurations sent from Calaos Installer, described in [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

Calling `calaos_config` with no argument displays the tool's help.
