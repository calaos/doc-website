+++
title = 'Codesys'
date = 2024-03-06T20:02:41Z
weight = 10
+++

## Programming Calaos on Wago PLCs

To operate a Wago PLC with Calaos, it must first be loaded with the Calaos program. There are two ways to program a PLC with Calaos:

- using Calaos Installer
- using Codesys

## IP Address

In any case, it's necessary to configure the PLC's IP address beforehand. It's advisable to refer to Wago's documentation for setting up a static IP address. By default, a PLC is set to DHCP. To switch to a static IP address, the Wago Ethernet Settings software should be used. This software also allows for searching the PLC on the network.

For PLCs equipped with DIP Switches on the front for IP setting, follow these steps:

- Set the PLC's selector to the middle position.
- Choose the IP address via the selectors. The address will be in the form of 192.168.1.XXX.
- Set the PLC's selector to the upper position.

To verify if the PLC is correctly configured on the network, simply access the PLC's web page via http://192.168.1.XXX. The web page should open.

## Calaos-Wago Program

First, download the Calaos code from [Github](https://github.com/calaos/calaos_wago/archive/master.zip). The archive contains all the code for all supported PLCs and the used libraries. Just extract the zip somewhere.

### Versions of calaos_wago

|Version|Notes|
|-|-|
|1.7 - 2.2| Old version, do not use|
|2.3| Stable version. Only works if no DALI Master 750-647 terminal is present|
|3.0| Stable version for use only with a DALI Master 750-647 terminal|

## Programming with Calaos Installer

The simplest solution is to use Calaos Installer. Just open the **Flash PLC with custom firmware** tool in the Wago menu. Then enter the IP of the PLC you wish to install. Calaos Installer will then ask you to select 2 files (use CTRL+select), a `.PRG` file and a `.CHK` file. Make sure to choose the correct version corresponding to your PLC and the desired version.

![img](/en/hardware/wago/images/wago_flash.png?width=20pc&classes=shadow)

{{% notice note %}}
Programming the PLC can take several minutes, be patient even if you don't see any progress.
{{% /notice %}}

## Programming with Codesys

Codesys is the PLC development tool. This tutorial uses version 2.3.9.xx. It's recommended to use the full version of Codesys, as the demo version does not allow the code to remain in the PLC after a restart.

{{% notice note %}}
It's advised to use Codesys in English, as the French version has some translations that are hard to understand. To change the language, go to Project Menu, Options, Working Environment, and choose English.
{{% /notice %}}

### Opening the project in Codesys

Start by running Codesys and then open the project corresponding to your PLC (e.g., wago_881.pro for a 750-881). Choose the latest version that can be found in the folder. E.g., Wago_3.0 for version 3.0 of the program.

Once the project is open, there should be no errors. An information message might pop up indicating that libraries have been modified, just validate it.

![img](/en/hardware/wago/images/codesys01.jpg?width=20pc&classes=shadow)

### Compiling the Project

To recompile the project, go to the **Project** menu then **Rebuild All**.

![img](/en/hardware/wago/images/codesys02.jpg?width=20pc&classes=shadow)

If everything goes well, the console at the bottom should show **0 Errors**.

![img](/en/hardware/wago/images/codesys03.jpg?width=20pc&classes=shadow)

### Selecting the PLC's IP

Next, go to **Online**, **Communication parameters**.

![img](/en/hardware/wago/images/codesys04.jpg?width=20pc&classes=shadow)

The following window will open:

![img](/en/hardware/wago/images/codesys05.jpg?width=20pc&classes=shadow)

Change the IP address in this window by double-clicking on it, then click OK.

### Loading

To load the PLC, go to **Online**, **Login**. The project will be compiled automatically if necessary, and the connection with the PLC will be established. If the connection works, Codesys will ask if you want to load the new program. Of course, respond **Yes**.

Loading can take some time (several minutes). Once finished, Codesys will switch to debuggermode, but the program is not executed. You can see whether the program is running at the bottom right.

![img](/en/hardware/wago/images/codesys07.jpg?width=20pc&classes=shadow)

Before executing the program, it must be written to non-volatile memory. Otherwise, everything will disappear after the next PLC reboot. To do this, once in **Online** mode, go to the **Online** menu, **Create boot project**.

![img](/en/hardware/wago/images/codesys08.jpg?width=20pc&classes=shadow)

This process will take some time, let Codesys complete its task.

## Calaos Installer

For Calaos Installer to work with the PLC, the PLC's LED must be steady orange. Do not connect to the PLC with Calaos Installer at the same time as `calaos_server`. It's essential to disconnect `calaos_server` from the network in this case.

In Calaos Installer:

- Go to the PLC menu → **Connect**, and enter the PLC's IP.
- Once connected, the bottom right of the window should display `connected (2.2)` with the PLC's version.
- If it's ok, the degraded mode configuration can be sent with PLC Menu → **Program the PLC**.
- The rules will work immediately without needing to restart the PLC.

For switch detection:

- You need to be connected to the PLC in Calaos Installer!
- If detection doesn't work, check that the LED on the input terminal lights up.
- If it still doesn't work, it might be a problem with Windows firewalls not letting UDP frames through correctly... Try disabling the firewall.
- If it still doesn't work, it's not a big deal; rules can still be created by specifying the input number to use for the switch. Count the input number directly on the terminal.
