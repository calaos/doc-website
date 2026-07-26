+++
title = 'Network'
date = 2024-03-06T20:15:08Z
weight = 10
summary = "IP address, DNS, Wi-Fi: configuring the server's network connection."
+++

## Automatic or fixed address

By default your server asks your router for an address: this is **automatic** mode, or DHCP. It works immediately, with nothing to configure.

The drawback is that this address can change, especially after a power cut or a router restart. Your applications, which look for the server at its old address, then no longer find it.

{{% notice tip %}}
**Give your server a fixed address.** You will always find it at the same place, and your applications will keep working after an outage. It is the only network setting that is really recommended.

There are two ways to get there: configure a fixed address on the server, as described below, or ask your router to always hand the same address to that machine — look for "static lease" or "DHCP reservation" in its interface.
{{% /notice %}}

## From the touchscreen

If a touchscreen is connected to the server, the **Calaos Home** interface lets you configure the network entirely from its configuration screen: choice between automatic and fixed address, entry of the address, netmask, gateway and DNS servers.

This is the simplest method, and it covers every case.

## Without a touchscreen

Connect to the server over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})).

### List the network interfaces

Start by looking at which connections the machine has:

```sh
calaos-os network list
```

The list shows the name of each interface. Spot the one you care about: names starting with `en` usually correspond to wired connections, those starting with `wl` to Wi-Fi.

### Switch to an automatic address

```sh
calaos-os network configure dhcp INTERFACE
```

replacing `INTERFACE` with the name found in the previous step.

### Configure a fixed address

```sh
calaos-os network configure static INTERFACE ADDRESS NETMASK GATEWAY DNS
```

For example, to give the address `192.168.1.50` to a machine whose router is at `192.168.1.1`:

```sh
calaos-os network configure static eth0 192.168.1.50 255.255.255.0 192.168.1.1 192.168.1.1
```

In this example:

- `192.168.1.50` is the address you choose for the server;
- `255.255.255.0` is the network mask; this value suits almost every home installation;
- `192.168.1.1` is your router's address, used both as gateway and as DNS server.

{{% notice warning %}}
Choose an address **outside the range handed out automatically by your router**, otherwise two devices could end up with the same address. That range is shown in your router's interface, often in the DHCP section.
{{% /notice %}}

You can list several DNS servers one after the other, separated by spaces.

## Check that it works

After the change, check that the server still sees the Internet:

```sh
ping -c 3 calaos.fr
```

Three replies mean everything is fine.

{{% notice info %}}
If you were connected over SSH and changed the server's address, **your connection is dropped**: that is normal, the server is no longer at the same address. Reconnect to the new one.
{{% /notice %}}

## The machine name

On your network the server also has a name, which sometimes lets you reach it without knowing its address. To see it:

```sh
hostname
```

## If you can no longer find your server

See the [Something went wrong]({{% relref "calaos_os/troubleshooting" %}}) page, which covers this case in detail.
