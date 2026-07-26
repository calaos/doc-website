+++
title = 'Services'
date = 2024-03-06T20:15:13Z
weight = 20
summary = "Viewing, restarting and monitoring the services running on the server."
+++

## What this is about

Your server permanently runs several background programs, called **services**: the Calaos server itself, but also the additional services such as MQTT or Zigbee, and system maintenance tasks.

In normal operation you do not have to deal with them: they start on their own at power-on and stop at shutdown. This page will mostly be useful the day something stops responding.

These settings are not available from the touchscreen: connect to the server over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})).

## The services you will come across

Here are the ones you will actually need, with their exact names:

| Service | Purpose |
|---|---|
| `calaos-server` | **The heart of your home automation**: it drives your devices and applies your rules |
| `calaos-home` | The local interface displayed on the touchscreen |
| `calaos-container` | The service that starts and supervises all the additional services |
| `mosquitto` | MQTT messaging |
| `zigbee2mqtt` | Zigbee gateway |
| `influxdb` | History of your measurements |
| `grafana` | Graphs |
| `haproxy` | Routing of incoming connections |
| `hue2mqtt` | Philips Hue gateway |
| `envoy` | Enphase solar inverters |
| `olad` | DMX lighting |

The most important service is `calaos-server`: if your home automation stops responding entirely, that is where to start.

```sh
systemctl status calaos-server
systemctl restart calaos-server
```

{{% notice tip %}}
`calaos-server` and `calaos-home` are two different things. If the touchscreen is frozen but your mobile applications still respond, `calaos-home` is the one to restart, and your home automation never stopped working.
{{% /notice %}}

## See what is running

To list the active services:

```sh
systemctl list-units --type=service
```

The left column gives the name of each service, and the following columns show whether it is working properly. Look for `failed`: that is the sign of a broken service.

To see only what is going wrong:

```sh
systemctl --failed
```

If this command returns nothing, all is well.

## One single way to do things, for every service

The additional services (MQTT, Zigbee, history, graphs…) run in containers, but **that changes nothing for you**: they are driven by the system like all the others.

In other words, `systemctl` sees and manages them in exactly the same way, and the commands on this page apply to them without distinction:

```sh
systemctl status zigbee2mqtt
systemctl restart zigbee2mqtt
```

So there are not two sets of commands to remember depending on the service.

These services are presented in detail on the [Services and modules]({{% relref "calaos_os/containers" %}}) page.

## Restart a service

If a service stops responding, restarting it is often enough to fix the problem:

```sh
systemctl restart SERVICE_NAME
```

replacing `SERVICE_NAME` with the name found in the list.

To see the detailed state of a service, and in particular the reason for a failure:

```sh
systemctl status SERVICE_NAME
```

The last log lines of the service are shown underneath, which usually gives a clue. See also [Logs]({{% relref "calaos_os/configuration/logs" %}}).

## Restart the whole server

When several things seem disturbed, a full restart is often quicker than diagnosing service by service:

```sh
reboot
```

Allow a minute before your home automation responds again.

## Going further

Calaos OS enables several automatic maintenance tasks, which run on their own and normally need no attention from you:

| Task | Purpose |
|---|---|
| `snapper-timeline.timer` | Regularly creates restore points |
| `snapper-cleanup.timer` | Deletes the oldest restore points |
| `btrfs-scrub@.timer` | Periodically checks the integrity of the data on the disk |
| `fstrim.timer` | Maintains SSD drives |
| `grub-btrfsd.service` | Keeps the list of restore points offered at boot up to date |

To enable or disable a service at boot:

```sh
systemctl enable SERVICE_NAME
systemctl disable SERVICE_NAME
```

{{% notice warning %}}
Do not disable the maintenance tasks above: they are what makes rollbacks possible and what keeps the disk from filling up with restore points.
{{% /notice %}}
