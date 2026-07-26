+++
title = 'Changing the passwords'
date = 2026-07-25T10:00:00Z
weight = 15
summary = "To be done right after installation: replace the two default passwords."
+++

## Why it matters

Calaos OS comes out of the installation with **two accounts protected by default passwords**. Those passwords are the same on every installation, and they are written on this page: anyone can know them.

Until you change them, anybody with access to your network can take control of your home automation. It only takes a few minutes, and it should be done **now**, right after the installation.

## The two accounts

| Account | What it is for | Default value |
|---|---|---|
| `root` | Administering the system: updates, configuration, troubleshooting | user `root`, password `calaos` |
| Calaos Server | Connecting to the server from Calaos Installer, the mobile applications and the web app | user `user`, password `pass` |

These really are **two different accounts**, with two different purposes. Change both.

## Connecting to the server

Both changes are made from the server, in a single session. If you have a screen and a keyboard connected to the machine, you can work directly on it. Otherwise, connect remotely from your everyday computer.

Open a terminal — the **Terminal** application on macOS and Linux, **PowerShell** or **Windows Terminal** on Windows — and type:

```sh
ssh root@YOUR_SERVER_ADDRESS
```

replacing `YOUR_SERVER_ADDRESS` with the IP address of your Calaos server. The password asked for is `calaos`.

{{% notice tip %}}
On the very first connection, your computer asks you to confirm that it does not know this machine yet. Answer `yes`. And do not be surprised: when you type a password in a terminal, **nothing is displayed**, not even asterisks. That is normal — type it and press Enter.
{{% /notice %}}

More details about this remote connection in [SSH access]({{% relref "calaos_os/advanced/shell" %}}).

## 1. Change the `root` password

Once connected, type:

```sh
passwd
```

The system asks for the new password, then asks you to confirm it a second time. Again, nothing is displayed while you type.

Choose a long password that you do not reuse anywhere else, and **write it down**: you will need it for every update and every intervention on the server.

## 2. Change the Calaos Server credentials

These are the ones Calaos Installer and the mobile applications ask for when they connect to your server.

### From the touchscreen

If your server has a touchscreen, the **Calaos Home** interface lets you change them directly from its configuration screen.

### Without a touchscreen

In the same session as before, type these two commands, replacing the values with your own:

```sh
calaos_config set cn_user my_user
calaos_config set cn_pass my_password
```

You can check the result with:

```sh
calaos_config get cn_user
```

The `calaos_config` tool is presented in detail on its [own page]({{% relref "calaos_os/configuration/calaos_config" %}}).

{{% notice warning %}}
After this change, **all your applications will refuse to connect** until you give them the new credentials: Calaos Installer, the mobile applications, the web app. Remember to update them straight away, otherwise you will think something is broken.
{{% /notice %}}

## If you forget a password

**The Calaos Server credentials** can be redefined from the server with the `calaos_config set` commands above: all you need is access to the `root` account.

**The `root` password**, however, can only be recovered with physical access to the machine, by booting from the Calaos OS USB stick. Store it in your password manager rather than relying on memory.

## Going further

For a server exposed to the Internet, replace the `root` password with **SSH key authentication**, which is far safer, then disable password login. The procedure is described in [SSH access]({{% relref "calaos_os/advanced/shell" %}}).

If you access your home automation from outside, also read [Dynamic DNS]({{% relref "calaos_os/configuration/calaos_ddns" %}}) before opening ports on your router.
