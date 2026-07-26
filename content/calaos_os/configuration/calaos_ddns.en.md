+++
title = 'Dynamic DNS'
date = 2024-03-06T20:16:11Z
weight = 80
summary = "Reaching your home automation from outside, with a calaos.fr address."
+++

## The problem it solves

At home, your applications find the Calaos server on the local network. From outside — at work, on holiday — you have to go through your Internet connection, and therefore know your router's public address.

The trouble is that this address **changes regularly**, without warning: most Internet providers do not guarantee a fixed address to home users. An application configured with the old address no longer finds anything.

Calaos provides a **dynamic DNS** service that solves this: you get a fixed address of the form `myhome.calaos.fr`, and your server reports its new address to the service on its own whenever it changes. You configure your applications once and for all with that name.

As a bonus, a **Let's Encrypt security certificate** is obtained automatically, which gives you an encrypted connection from outside.

## Before you start

**Your router must forward connections to your server.** By default a router blocks everything coming from outside. You have to tell it to send incoming connections to your Calaos server: this is called **port forwarding**, configured in your router's interface.

**Your server must have a fixed address on the local network**, otherwise the forwarding will one day point at another device. See [Network]({{% relref "calaos_os/configuration/network" %}}).

{{% notice warning %}}
Opening access from the Internet makes your home automation **reachable by anyone**, not just by you.

Before doing so, make sure you have changed the default passwords — see [Changing the passwords]({{% relref "calaos_os/security" %}}) — and choose a long password that you use nowhere else. That is the absolute minimum.
{{% /notice %}}

## Registering your domain name

Connect to the server over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})), then choose the name you want to use:

```sh
calaos_ddns register myhome
```

This command registers **`myhome.calaos.fr`** and associates it with the public address of your connection.

Choose a name that identifies you but stays discreet: it will be visible from outside, so avoid your family name or your street address.

### The security certificate

On the first registration, the tool asks for **an email address**. It is used only to obtain the Let's Encrypt certificate, which encrypts the exchanges between your applications and your server.

```text
To generate a Let's Encrypt certificate, you need to set a user email address.
Enter your email address:
```

Enter a valid address and confirm. Generating the certificate takes a few moments.

{{% notice note %}}
If you leave the field empty, registration still works, but **no certificate is generated** and your connection from outside will not be encrypted. This is not recommended.
{{% /notice %}}

## Adding subdomains

You can register additional names under your main domain, for instance to reach a camera:

```sh
calaos_ddns register myhome camera1
```

creates `camera1.myhome.calaos.fr` in addition to `myhome.calaos.fr`.

Better still: by giving the address and port of the device, the server takes care of routing the connection to it.

```sh
calaos_ddns register myhome camera1=192.168.0.20:4444
```

Visiting `camera1.myhome.calaos.fr` will then be routed to the device at `192.168.0.20:4444` on your local network. You can list several subdomains one after the other, separated by spaces.

{{% notice tip %}}
This routing is handy, but remember that **the target device becomes reachable from the Internet**. Only use it for devices whose password you control, and never for a camera still on its factory credentials.
{{% /notice %}}

## Updates are automatic

Once registration is done, you have nothing more to do: your server checks **every two hours** that its public address has not changed, and tells the service if it has. The certificate is renewed the same way.

To force an immediate check, for instance right after your router restarted:

```sh
calaos_ddns update
```

## Checking that it works

The test must be done **from outside your home**: turn off your phone's Wi-Fi to switch to mobile data, then try to connect to your server using your `.calaos.fr` name.

{{% notice tip %}}
Many routers cannot forward a connection out and then back in. In other words, testing your name from home may fail while everything works perfectly from outside. Always test from a mobile connection.
{{% /notice %}}

## The other commands

| Command | Effect |
|---|---|
| `calaos_ddns register DOMAIN [SUBDOMAIN...]` | Registers a name and obtains a certificate |
| `calaos_ddns update` | Forces an address update |
| `calaos_ddns update -f` | Also forces the certificate to be renewed |
| `calaos_ddns unregister` | Removes the registration |
| `calaos_ddns reset` | Clears the token stored on the server |

## The token is no longer valid

Your server keeps an **identification token**, obtained during registration, which lets it prove to the service that the domain is really its own. If your server stayed offline for a very long time, this token can become invalid.

You notice it because the `calaos_ddns` commands **return an error** instead of working, including `unregister` — since that one starts by presenting the token to the service, which rejects it.

The fix is to **delete the old token** from your server's configuration:

```sh
calaos_config del ddns_token
```

Then register again as normal:

```sh
calaos_ddns register myhome
```

{{% notice note %}}
`calaos_ddns reset` does exactly the same thing as the `calaos_config del` command above. If `reset` fails too, go straight to `calaos_config del ddns_token`, which simply clears the setting locally without asking the service anything.
{{% /notice %}}

{{% notice warning %}}
This clears the local trace of the registration, but **not the registration itself on the service side**. If you take the same domain name again, everything falls back into place. If you change it, the old name may stay reserved for a while.
{{% /notice %}}

## If it does not work

Check in this order:

- is your server reachable normally **from the local network**? If not, the problem is upstream;
- is port forwarding correctly configured on your router, towards the server's current local address?
- has the server's local address changed since you configured the forwarding?
- does your provider give you a shared public address? Some plans, particularly on fibre or 4G, do not allow incoming connections. In that case no setting will work and you must ask your provider for a public address.

To see what the update service is doing:

```sh
systemctl status calaos-ddns.timer
journalctl -u calaos-ddns.service
```

## Going further

The update service is triggered by `calaos-ddns.timer`, one minute after boot and then every two hours.

Subdomain routing is handled by HAProxy, whose configuration is generated automatically in `/mnt/calaos/haproxy`, with the certificate in `server.pem`. These files are part of your data and are therefore kept across updates.

The identification token and the email address are stored in the server configuration, under the keys `ddns_token` and `ddns_le_email` — see [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).

## See also

To configure your mobile applications with your remote access, see [Upload the project]({{% relref "getting_started/upload" %}}).
