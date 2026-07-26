+++
title = 'Touchscreen'
date = 2024-03-06T20:15:39Z
weight = 50
summary = "The screen connected to the server and the Calaos Home interface."
+++

## What the touchscreen is for

A touchscreen connected directly to the server displays **Calaos Home**, the local Calaos interface. It shows your home — rooms, lights, shutters, temperatures — and lets you control it with your finger.

It is also used for a few operations on the system itself:

- configuring the [network]({{% relref "calaos_os/configuration/network" %}});
- changing the [login credentials]({{% relref "calaos_os/security" %}});
- changing its own [language]({{% relref "calaos_os/configuration/lang" %}});
- [installing Calaos OS]({{% relref "calaos_os/installation" %}}) onto the disk, from Live mode;
- confirming a [rollback]({{% relref "calaos_os/rollback" %}}).

{{% notice info %}}
**The touchscreen is optional.** Without it your server works exactly the same: you control your home from the mobile applications or the web app, and you configure the server remotely over SSH.
{{% /notice %}}

## Server screen or wall screen?

Do not confuse two different things:

- **the screen connected to the server**, which this page is about. There is only one, where the machine is;
- **the Remote UI wall screens**, spread across the rooms of the house, which connect to the server over the network.

If your goal is to have wall controls in several rooms, Remote UI screens are what you want: see [Remote UI]({{% relref "hardware/remote_ui" %}}).

## Setting it up

Connect the screen to the server's video output, and its touch cable to a USB port, before switching the machine on. At boot, Calaos Home is displayed automatically.

If you connect the screen while the server is already running, restart it so that the screen is detected:

```sh
reboot
```

## If the screen stays black

Check in this order:

1. **is the screen powered and switched on?** Many screens have their own switch;
2. **is the video cable properly connected** at both ends, and to the right output if the machine has several;
3. **is the screen on the right input?** A screen set to HDMI 2 while you are plugged into HDMI 1 stays black;
4. **is the server running anyway?** Try connecting to it over SSH: if that works, the problem is purely a display one.

To understand what is happening on the display side, check the [logs]({{% relref "calaos_os/configuration/logs" %}}):

```sh
journalctl -b | grep -i -E "drm|display|xorg"
```

## Calibrating touch

If touch works but lands next to where you press — you tap a button and its neighbour reacts — the screen needs calibrating.

Connect over SSH and run:

```sh
xinput_calibrator
```

A calibration screen appears: touch each cross as it shows up in the corners. At the end, **the values are saved automatically in the right place**, in `/mnt/calaos/xorg.conf.d`. There is no file to copy or edit.

Then restart so the new calibration is taken into account:

```sh
reboot
```

{{% notice tip %}}
Touch the crosses as precisely as you can, with a fingertip or a stylus, looking straight at the screen. A calibration done at an angle gives a touch that is off near the edges.
{{% /notice %}}

## If touch does not respond at all

If the picture is displayed but touch does nothing, the touch part is not recognised. It usually goes through a cable **separate** from the video cable: check that it is properly connected.

To see whether the system detects a touch device:

```sh
libinput list-devices
```

### Touchscreen connected through a USB / serial adapter

Some screens, particularly resistive ones, do not plug in directly over USB: they use a serial link, connected to the computer through a **USB/serial adapter**. These screens are not detected automatically, so the matching service must be enabled.

First find the name of the device created by the adapter:

```sh
ls /dev/ttyUSB*
```

It is most often `ttyUSB0`. Then enable the service with that name, and start it:

```sh
systemctl enable usb-serial-touchscreen@ttyUSB0.service
systemctl start usb-serial-touchscreen@ttyUSB0.service
```

If your adapter appears under another name — `ttyUSB1` for instance — replace it in both commands.

Once the service is active, touch is recognised and you can calibrate it as described above.

To check that the service runs correctly:

```sh
systemctl status usb-serial-touchscreen@ttyUSB0.service
```

## Going further

The display and calibration settings specific to your installation are kept in `/mnt/calaos/xorg.conf.d`. As this folder is part of your data, **its contents survive updates** — see [Backing up your configuration]({{% relref "calaos_os/backup" %}}).

{{% notice note %}}
Touchscreens vary a great deal, so some fine settings — display rotation, sleep mode — depend on the model you use. If you run into difficulty with a particular screen, the Calaos forum is the best place to find a configuration that already works.
{{% /notice %}}
