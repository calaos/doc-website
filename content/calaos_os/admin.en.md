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

Your browser will probably show a warning such as "Your connection is not private". This is expected: the box creates its own security certificate, which no public authority has signed. The traffic is encrypted all the same. Accept the warning once (usually under "Advanced", then "Continue to ...").

If you open the Admin tab on an address that does not start with `https://`, the web app tells you to use `https://` and offers an **Open in https://** button.

## First use: choose the admin password

1. Open the **Admin** tab.
2. If the tab shows **Change your sign-in password first**, fill in the form right there (user name, new password twice) and click **Save and continue**. A box that still uses the default `user` / `pass` login cannot get an admin password: those values are known to everybody. This is your Calaos Server password, see [Change the Calaos Server credentials]({{% relref "calaos_os/security#2-change-the-calaos-server-credentials" %}}).
3. Type an admin password of **at least 10 characters**, twice, and click **Set the admin password**.

{{% notice warning %}}
Once the sign-in password has changed, Calaos Installer, the mobile applications and the web app all need the new one.
{{% /notice %}}

The admin password is not stored anywhere readable, so **write it down** in your password manager. If you lose it, see [If you lose the admin password](#if-you-lose-the-admin-password).

{{% notice warning %}}
Choose the admin password yourself, soon after installation. Until one is set, anyone who knows your login password can set it first. If you ever find that someone else did, the recovery below puts things back.
{{% /notice %}}

## Unlock the Admin tab

When the Admin tab asks for it (the first time, then every 15 minutes, or after reloading the page), type the admin password and press **Unlock**. After several wrong passwords, the web app makes you wait a little (up to a minute) and shows how long.

## Install updates

1. Click **Check for updates**. This contacts the Calaos servers and can take up to a minute.
2. If updates are available, click **Install updates**.
3. A progress bar shows the package being installed. Keep the box powered on.

The box stays usable during the update. The web app may lose its connection for a moment while the server restarts, and reconnects by itself.

When it is done, the web app says **Update finished**. If it says **Update finished — restart the box**, part of what was installed only takes effect after a restart: click **Restart the box** in the **Box** section of the tab, then **Restart** to confirm. It takes about a minute and the web app reconnects when the box is back.

{{% notice tip %}}
Updating is as safe as with the terminal: a restore point is created before each installation. See [Rolling back]({{% relref "calaos_os/rollback" %}}).
{{% /notice %}}

### The very first upgrade

A box that does not have the Admin tab yet is brought up to date with the usual tools: `calaos-os upgrade`, `apt upgrade` (see [Update]({{% relref "calaos_os/update" %}})), or the update button of the touchscreen. Nothing else to do: once that upgrade is done, the Admin tab works. For a short time the web app can show **Administration is being activated** while the box finishes: it retries on its own. Only if it still shows **Restart the box** after a couple of minutes, restart the box yourself, since the Admin tab cannot do it in that state: unplug it and plug it back in, or type `reboot` over SSH (see [Services]({{% relref "calaos_os/configuration/services" %}})).

## Change the admin password

In the unlocked Admin tab, go to the **Box** section, **Admin password**, and click **Change**. You stay unlocked with the new password.

This change also needs a code sent to your approver phone, so enrol one first: see [Approver phone and web console]({{% relref "calaos_os/approver" %}}). That page also covers the web console. Without an approver phone, use `calaos-os admin set` over SSH (below).

## If you lose the admin password

Connect to the box over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})) as `root`, and type:

```sh
calaos-os admin set
```

The command asks for the new admin password, twice. It replaces the old one, whether or not you remember it. Anyone logged in to the Admin tab has to unlock again.

`calaos-os admin set` keeps the approver phones. To remove the admin password altogether, **and the approver phones**, so that the web app offers to choose a new one:

```sh
calaos-os admin reset
```

After `calaos-os admin reset`, anyone who knows your sign-in password could choose the new admin password first. Change the Calaos Server password first, then choose the new admin password in the web app.

Both commands need `root` access to the box, which is why no web screen can do the same thing.

{{% notice note %}}
A [rollback]({{% relref "calaos_os/rollback" %}}) also restores the admin password (and the login password) that existed at the date you went back to.
{{% /notice %}}

## Allowing other host names: `api_allowed_hosts`

Until the admin password is set, nothing is checked. Once it is set, the web app is only accepted when you open it with one of the box's own IP addresses (including on port 5454), or with any name in `https://`. Without https, for example `http://calaos.local:5454`, any other name, including `calaos.local` or the box's host name, is refused unless you add it to the `api_allowed_hosts` option: a list of host names separated by commas. An entry may end with a port, for example `calaos.example.net:8080`; without a port, any port is accepted.

```sh
calaos_config set api_allowed_hosts "calaos.example.net,box.local"
```

(see [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).) Set it **before** choosing the admin password if you can: the screen where you choose the password shows the current list so that you can check it.

{{% notice note %}}
If you edited the `haproxy.cfg` file by hand, keep the `http-request set-header X-Forwarded-Proto` line in each frontend. Without it the box cannot tell that the connection is secure.
{{% /notice %}}

## Behind an external reverse proxy

*For advanced users.* If another machine of your network sits in front of the box (HAProxy, nginx...), it must forward to the box's HAProxy on port **443 over TLS**, not to port 5454 over plain http: with 5454 the web app shows "Use https://", because the admin password would otherwise travel in clear on your network. Your browser only sees the certificate of the external proxy; the box's certificate is only checked by that proxy. The reason: calaos_server only accepts admin actions from a loopback connection carrying `X-Forwarded-Proto: https`, which only the box's own HAProxy sets; `X-Forwarded-*` headers from your external proxy are therefore optional. The original `Host` header must be passed through: it keeps the Origin check working, so `api_allowed_hosts` is not needed.

If the box has a valid Let's Encrypt certificate through [calaos_ddns]({{% relref "calaos_os/configuration/calaos_ddns" %}}) (for example `mabox.calaos.fr`), check it with the system certificates and the box's DDNS name. HAProxy example:

```
backend calaos
    server calaos-server <box-ip>:443 ssl sni str(mabox.calaos.fr) verify required ca-file ca-certificates.crt verifyhost mabox.calaos.fr check check-sni mabox.calaos.fr
```

Here `ca-file ca-certificates.crt` assumes `ca-base /etc/ssl/certs` in the `global` section; `@system-ca` does not work in that case. Without DDNS, the box has a self-signed certificate: replace the `ssl ...` options with `ssl verify none` (or give the box certificate to `ca-file` to pin it).

The nginx equivalent (the `map` goes in the `http` block):

```
map $http_upgrade $connection_upgrade {
    default upgrade;
    ''      close;
}

location / {
    proxy_pass https://<box-ip>:443;
    proxy_ssl_server_name on;
    proxy_ssl_name mabox.calaos.fr;
    proxy_ssl_verify on;
    proxy_ssl_verify_depth 2;
    proxy_ssl_trusted_certificate /etc/ssl/certs/ca-certificates.crt;
    proxy_set_header Host $host;
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection $connection_upgrade;
    proxy_read_timeout 1h;
}
```

(with a self-signed certificate, use `proxy_ssl_verify off;` and drop the two other `proxy_ssl_` lines for the name and the CA.)

{{% notice note %}}
If the box fails to renew its Let's Encrypt certificate, it falls back to a self-signed one and the proxy refuses the connection. This is on purpose: no silent downgrade of the check.
{{% /notice %}}

## Docker and other installations

There is **no web administration** on Docker installations nor on systems other than Calaos OS. The Admin tab says **Administration unavailable**, and nothing else changes: everything else works as before. Update those installations with their usual tools (for Docker, pull the new images).

## Calaos Installer

If your box has an admin password, **Load the project** and **Send the project** (**Calaos Server** menu) in Calaos Installer also need it: an optional **Admin password** field appears in both windows. Leave it empty if the box has none. It is never saved, so you type it each time you open the window. See [Calaos Installer]({{% relref "calaos_installer" %}}).
