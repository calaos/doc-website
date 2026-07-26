+++
title = 'SSH access'
date = 2026-07-25T10:00:00Z
weight = 20
summary = "Connecting to the server remotely, and useful commands."
+++

## Connecting

SSH lets you take control of your server from another computer on the network. It is the normal way to administer Calaos OS.

Open a terminal — the **Terminal** application on macOS and Linux, **PowerShell** or **Windows Terminal** on Windows — then:

```sh
ssh root@YOUR_SERVER_ADDRESS
```

The password is the one you set in [Changing the passwords]({{% relref "calaos_os/security" %}}). By default it is `calaos`, which is only acceptable for as long as it takes you to change it.

{{% notice tip %}}
On the first connection your computer asks you to confirm the machine's fingerprint: answer `yes`. And while you type the password, **nothing is displayed** — that is normal.
{{% /notice %}}

To disconnect:

```sh
exit
```

## Key authentication

Safer and more convenient than a password, it is recommended as soon as your server is reachable from outside.

On your computer, if you do not have a key yet:

```sh
ssh-keygen -t ed25519
```

Then install it on the server:

```sh
ssh-copy-id root@YOUR_SERVER_ADDRESS
```

Subsequent connections no longer ask for a password.

Once the key is confirmed working, you can forbid password login by adding `PasswordAuthentication no` to `/etc/ssh/sshd_config`, then restarting the service:

```sh
systemctl restart sshd
```

{{% notice warning %}}
Check that your key works **before** disabling passwords, by opening a second session without closing the first. Otherwise a configuration mistake would lock you out, and you would need a screen and a keyboard on the machine to recover.
{{% /notice %}}

## Without a network

If the server is no longer reachable, connect a screen and a keyboard directly to it: you get a local terminal, where all the commands in this documentation work identically.

## Useful commands

### System state

```sh
systemctl --failed          # failed services
systemctl status SERVICE    # detailed state of a service
systemctl restart SERVICE   # restart a service
reboot                      # restart the machine
```

### Logs

```sh
journalctl -f               # follow the logs live
journalctl -u SERVICE       # logs of one service
journalctl -b               # logs of the last boot
```

See [Logs]({{% relref "calaos_os/configuration/logs" %}}).

### Additional services

```sh
podman ps                   # running containers
podman logs NAME            # logs of a container
```

See [Services and modules]({{% relref "calaos_os/containers" %}}).

### Disk and restore points

```sh
df -h /                     # free space
btrfs filesystem usage /    # detailed disk usage
snapper list                # list of restore points
```

See [Under the hood]({{% relref "calaos_os/advanced/btrfs" %}}).

### System and network

```sh
calaos-os list              # installed versions
calaos-os network list      # network interfaces
calaos_config list          # Calaos server settings
hostname                    # machine name
timedatectl                 # date, time and time zone
```

## Copying files

To fetch a file from the server:

```sh
scp root@YOUR_SERVER_ADDRESS:/tmp/journal.txt .
```

To send one to the server:

```sh
scp file.txt root@YOUR_SERVER_ADDRESS:/tmp/
```

## Good practice

{{% notice warning %}}
**Your manual changes can be overwritten by an update.** System files belong to the installed packages: if you edit a configuration file by hand, the next update of that package may replace it.

Two exceptions: the `/mnt/calaos` folder, which holds your data and is never touched, and the settings made with `calaos_config` or `calaos-os`, which are designed to last.
{{% /notice %}}

Finally, remember that a rollback restores **the whole system, including `/mnt/calaos`**: it will therefore repair a bad move, but at the same time undo the configuration done since the chosen restore point. See [Rolling back]({{% relref "calaos_os/rollback" %}}) and [Backing up your configuration]({{% relref "calaos_os/backup" %}}).
