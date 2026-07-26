+++
title = 'Date & Time'
date = 2024-03-06T20:15:56Z
weight = 60
summary = "Time zone and automatic clock synchronisation of the server."
+++

## Why it matters

The server's time is not a comfort detail. It governs the correct behaviour of your installation:

- **your time-based rules** — switch on at 7 pm, close the shutters at sunset — trigger according to the server's time;
- **sunrise and sunset** are computed from the date;
- **Remote UI screens** refuse to connect if the time is wrong, since their security relies on it;
- **your measurement history** is timestamped with this time.

A shifted clock produces confusing symptoms: rules triggering at the wrong moment, or devices refusing to connect for no apparent reason.

These settings are not available from the touchscreen: connect over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})).

## Check the current time

```sh
timedatectl
```

This command shows the local time, the configured time zone, and whether automatic synchronisation is active.

Check three things: that the displayed time is correct, that the zone matches your country, and that automatic synchronisation is enabled.

## Change the time zone

To see the list of available zones:

```sh
timedatectl list-timezones
```

The list is long; to keep only European ones:

```sh
timedatectl list-timezones | grep Europe
```

Then apply yours:

```sh
timedatectl set-timezone Europe/Paris
```

## Automatic clock synchronisation

Your server adjusts its clock by querying time servers on the Internet. This is what keeps it on time over the long run, including across daylight saving changes.

To check or enable it:

```sh
timedatectl set-ntp true
```

{{% notice tip %}}
If the time is systematically wrong after every power cut, the **motherboard battery** is often dead. It is a cheap coin cell, easy to replace. Automatic synchronisation fixes the problem as soon as the network is available, but a few minutes may pass before that.
{{% /notice %}}

## The position of your home

Time alone is not enough to know when the sun rises: you also need to know **where you are**. Without that information, rules such as "close the shutters at sunset" cannot work correctly.

So set the coordinates of your home:

```sh
calaos_config set latitude 47.636507
calaos_config set longitude 7.494156
```

Replace these values with your own. You can get them easily by looking your address up on any online map: both numbers are displayed, latitude first.

{{% notice tip %}}
A precision of a few decimal places is more than enough — there is no need to be accurate to the metre. Do watch the **decimal point** though: write `47.636507`, not `47,636507`.
{{% /notice %}}

To check what has been saved:

```sh
calaos_config get latitude
calaos_config get longitude
```

Using sunrise and sunset in your rules is described in [Time management]({{% relref "calaos_installer/time" %}}).

## After a change

Restart the Calaos server so the new setting is taken into account everywhere, or simply restart the machine:

```sh
reboot
```

## See also

Time handling in your rules — time ranges, calendars, sunrise and sunset — is described in [Time management]({{% relref "calaos_installer/time" %}}), in the Calaos Installer chapter.

For wall screens and their requirement of a correct clock, see [Remote UI]({{% relref "hardware/remote_ui" %}}).
