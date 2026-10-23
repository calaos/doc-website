+++
title = 'Administering from the web app'
date = 2026-10-23T06:00:00Z
weight = 22
summary = "Install updates and restart the box from the Calaos web app, without SSH."
+++

## What this is about

You can install updates and restart your box from the Calaos **web app**, in its **Admin** tab. No terminal and no SSH needed.

These actions are protected by an **admin password**, separate from the password you use to log in to Calaos. The admin password is only asked for when you open the Admin tab.

{{% notice info %}}
This is available on **Calaos OS** only. See [Docker and other installations](#docker-and-other-installations) below.
{{% /notice %}}

## Open the web app over https

The Admin tab only works on a secure connection. Open the web app with an address starting with **`https://`**, for example `https://192.168.1.50` (your box's IP address), not `http://`.

Your browser will show a warning such as "Your connection is not private". This is expected: the box creates its own security certificate, which no public authority has signed. The traffic is encrypted all the same. Accept the warning once (usually under "Advanced", then "Continue to ...").

If you open the Admin tab on an address that does not start with `https://`, the web app tells you to use `https://`.

## First use: choose the admin password

1. Open the **Admin** tab.
2. If the web app asks you to **change your login password first**, do it. A box that still uses the default `user` / `pass` login cannot get an admin password: those values are known to everybody. See [Changing the passwords]({{% relref "calaos_os/security" %}}).
3. Type an admin password of **at least 10 characters**, twice, and confirm.

The admin password is not stored anywhere readable, so **write it down** in your password manager. If you lose it, see [If you lose the admin password](#if-you-lose-the-admin-password).

{{% notice warning %}}
Choose the admin password yourself, soon after installation. Until one is set, anyone who knows your login password can set it first. If you ever find that someone else did, the recovery below puts things back.
{{% /notice %}}

## Unlock the Admin tab

Each time you open the Admin tab, type the admin password and press **Unlock**. The unlock lasts **15 minutes** and then the web app asks for the password again. After a wrong password, wait a minute before trying again.

## Install updates

1. Click **Check for updates**. This contacts the Calaos servers and can take up to a minute.
2. If updates are available, click **Install updates**.
3. A progress bar shows the package being installed. Keep the box powered on.

The box stays usable during the update. The web app may lose its connection for a moment while the server restarts, and reconnects by itself.

When it is done, the web app says **Update complete**. If it says **Restart the box**, part of what was installed only takes effect after a restart: click **Restart the box** at the bottom of the tab, and confirm. It takes about a minute and the web app reconnects when the box is back.

{{% notice tip %}}
Updating is as safe as with the terminal: a restore point is created before each installation. See [Rolling back]({{% relref "calaos_os/rollback" %}}).
{{% /notice %}}

### The very first upgrade

A box that does not have the Admin tab yet is brought up to date with the usual tools: `calaos-os upgrade`, `apt upgrade` (see [Update]({{% relref "calaos_os/update" %}})), or the update button of the touchscreen. Nothing else to do: once that upgrade is done, the Admin tab works. For a short time the web app can show **Admin activation in progress** while the box finishes: it retries on its own. Only if it still shows **Restart the box** after a couple of minutes, restart the box.

## If you lose the admin password

Connect to the box over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})) as `root`, and type:

```sh
calaos-os admin set
```

The command asks for the new admin password, twice. It replaces the old one, whether or not you remember it. Anyone logged in to the Admin tab has to unlock again.

To remove the admin password altogether, so that the web app offers to choose a new one:

```sh
calaos-os admin reset
```

Both commands need `root` access to the box, which is why no web screen can do the same thing.

{{% notice note %}}
A [rollback]({{% relref "calaos_os/rollback" %}}) also restores the admin password (and the login password) that existed at the date you went back to.
{{% /notice %}}

## Allowing other host names: `api_allowed_hosts`

Once the admin password is set, the web app is only accepted from the box's own IP addresses and from the name you use through its https front end. If you reach the web app under another name, for example a home DNS name or a `.local` name, add it to the `api_allowed_hosts` option: a list of host names separated by commas.

```sh
calaos_config set api_allowed_hosts "calaos.example.net,box.local"
```

(see [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).) Set it **before** choosing the admin password if you can: the screen where you choose the password shows the current list so that you can check it.

{{% notice note %}}
If you edited the `haproxy.cfg` file by hand, keep the `http-request set-header X-Forwarded-Proto` line in each frontend. Without it the box cannot tell that the connection is secure.
{{% /notice %}}

## Docker and other installations

There is **no web administration** on Docker installations nor on systems other than Calaos OS. The Admin tab says **Administration unavailable**, and nothing else changes: everything else works as before. Update those installations with their usual tools (for Docker, pull the new images).

## Calaos Installer

If your box has an admin password, **Load Project from Calaos Server** and **Send the project** in Calaos Installer also need it: an optional **Admin password** field appears in both windows. Leave it empty if the box has none. It is never saved, so you type it each time you open the window. See [Calaos Installer]({{% relref "calaos_installer" %}}).
