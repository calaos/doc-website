+++
title = 'Calaos Config'
date = 2024-03-06T20:18:15Z
weight = 60
summary = "The configuration files: format, location and life cycle."
+++

## Three files

The whole configuration of an installation fits in three XML files, stored in `/mnt/calaos/config`:

| File | Contents |
|---|---|
| `io.xml` | The inputs/outputs, organised by room |
| `rules.xml` | The rules and scenarios |
| `local_config.xml` | The settings of the server and the local interface |

The first two describe **your home** and are produced by Calaos Installer. The third holds the **machine settings** and is handled with the `calaos_config` tool.

## io.xml and rules.xml

These files are written by Calaos Installer, then sent to the server which reloads them. They are what you edit indirectly every time you add a device or a rule.

On the server side, `CalaosConfig` loads them at startup: `IOFactory` instantiates a class for each declared IO, based on its type attribute, and the rules engine builds its conditions and actions from `rules.xml`.

This is why adding a new IO type to the server is enough to make it declarable in these files — see [Calaos Server]({{% relref "dev/calaos_server" %}}).

{{% notice warning %}}
**Do not edit these files by hand while the server is running.** The next upload from Calaos Installer will overwrite your changes, and a malformed file will prevent the server from starting.
{{% /notice %}}

## local_config.xml

This file stores key/value pairs, with no imposed structure. It is read and written:

- by **`calaos_server`**, through `Utils::get_config_option()` and `set_config_option()`;
- by the **`calaos_config` tool**, which is only a command line interface over those same functions;
- by **Calaos Home**, which stores its own display settings there;
- by **`calaos_ddns`**, which keeps its token there.

The list of keys is described in [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).

The tool's implementation is about a hundred lines, in `src/bin/tools/calaos_config.cpp`. It exposes four actions — `list`, `get`, `set`, `del` — and calls `sync()` after writing to make sure the value reaches the disk.

{{% notice tip %}}
This simplicity is deliberate: **adding a setting requires no change to the tool**. You just read the new key where you need it, and `calaos_config set` already knows how to write it.
{{% /notice %}}

## Configuration history

Every time a new configuration is sent from Calaos Installer, the server archives the previous one before replacing it, in a dated tree:

```text
/mnt/calaos/config/backups/2026/07/01-07-2026_16-56-00/
├── io.xml
├── local_config.xml
└── rules.xml
```

This mechanism allows going back to an earlier version. It is not a backup for all that, since it lives on the same machine and in the same folder — see [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

## Credentials

The server login credentials are stored under the `cn_user` and `cn_pass` keys. The historical `calaos_user` and `calaos_password` keys are still read as a fallback, and deleted as soon as the credentials are changed — see `JsonApi::changeCredentials`.

The change can be made in three ways: from the Calaos Home interface, through the API with `change_cred`, or on the command line with `calaos_config set`. See [Changing the passwords]({{% relref "calaos_os/security" %}}).

## Location

Everything lives under `/mnt/calaos`, alongside the data of the additional services. This folder is **the only one to back up**, and it is never touched by a system update.

It is however part of the root subvolume, and therefore of a restore point: a rollback replays the configuration too. See [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).
