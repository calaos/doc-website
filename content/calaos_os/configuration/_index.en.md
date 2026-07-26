+++
title = 'Configuration'
date = 2024-03-06T20:14:53Z
weight = 30
summary = "Setting up the network, services, date, language and notifications."
+++

## Two ways to configure your server

**From the touchscreen.** If a touchscreen is connected to the server, the **Calaos Home** interface offers a configuration screen.

**Remotely, over SSH.** From your everyday computer you connect to the server and type commands. This method is available in every case.

{{% notice info %}}
**The touchscreen is optional, and not all options are available there yet.** Today, Calaos Home lets you configure the network, the login credentials and its own language. Everything else goes through the command line.

If you have no screen, you lose nothing essential: the SSH route covers every setting.
{{% /notice %}}

Each page below states which of the two methods is available for that particular setting.

## Before you start

For the SSH route you will need your server's `root` password: see [Changing the passwords]({{% relref "calaos_os/security" %}}) if you have not set it yet, and [SSH access]({{% relref "calaos_os/advanced/shell" %}}) for the connection itself.

Most settings go through the `calaos_config` tool, presented on its [own page]({{% relref "calaos_os/configuration/calaos_config" %}}).

{{% children description="true" %}}
