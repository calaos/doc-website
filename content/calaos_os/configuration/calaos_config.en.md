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

## The available settings

### Server connection

| Key | Purpose |
|---|---|
| `cn_user` | User name used to connect to the server |
| `cn_pass` | Password used to connect to the server |

See [Changing the passwords]({{% relref "calaos_os/security" %}}).

### Geographic position

| Key | Purpose |
|---|---|
| `latitude` | Latitude of your home |
| `longitude` | Longitude of your home |

These two values let the server compute sunrise and sunset times, which are essential to the time rules that depend on them. See [Date & Time]({{% relref "calaos_os/configuration/date" %}}).

### Sending emails

| Key | Purpose |
|---|---|
| `smtp_server` | Address of the sending server, in the form `smtp://server` |
| `smtp_port` | Port of the sending server |
| `smtp_auth` | `true` if the server requires authentication |
| `smtp_tls` | `true` for an encrypted connection |
| `smtp_username` | User name |
| `smtp_password` | Password |

See [Email]({{% relref "calaos_os/configuration/email" %}}).

### Zigbee device notifications

| Key | Purpose |
|---|---|
| `notif/battery_mail_enabled` | Warn by email when a battery runs low |
| `notif/battery_push_enabled` | Warn by mobile notification when a battery runs low |
| `notif/io_connected_mail_enabled` | Warn by email when a device connects or disconnects |
| `notif/io_connected_push_enabled` | Warn by mobile notification when a device connects or disconnects |

All of them expect `true` or `false`. Sending emails requires the sending server to be configured first. See [Zigbee]({{% relref "hardware/zigbee" %}}).

### Calaos Home interface

| Key | Purpose |
|---|---|
| `calaos_server_host` | Forces the interface to connect to a specific server, and disables automatic discovery. An IP address is enough |
| `show_cursor` | Whether the mouse cursor is displayed on screen |
| `dpms_enable` | Enables automatic screen sleep |
| `dpms_standby` | Delay before the screen goes to sleep, in minutes |

See [Touchscreen]({{% relref "calaos_os/configuration/touchscreen" %}}).

### Diagnostic logs

| Key | Purpose |
|---|---|
| `debug_enabled` | Enables the server's detailed logs |
| `debug_level` | Global level of detail |
| `debug_domains` | Level of detail per domain, in the form `domain:level,domain:level` |

Only enable these while diagnosing a problem. See [Logs]({{% relref "calaos_os/configuration/logs" %}}).

---

This list covers the most common settings. `calaos_config list` shows you the ones actually defined on your installation.

{{% notice warning %}}
**Do not edit the configuration files by hand** while the server is running. Your changes may be overwritten, and a malformed file will prevent the server from starting. Always go through `calaos_config`.
{{% /notice %}}

## Going further

Settings are stored in the `local_config.xml` file, in `/mnt/calaos/config`. That folder also holds the history of the configurations sent from Calaos Installer, described in [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

Calling `calaos_config` with no argument displays the tool's help.
