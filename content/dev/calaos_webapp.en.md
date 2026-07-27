+++
title = 'Calaos WebApp'
date = 2024-03-06T20:18:10Z
weight = 40
summary = "The interface reachable from a browser."
+++

## Role

The WebApp is the Calaos interface reachable from a **browser**, with nothing to install. It is useful from a computer you do not own, or from a device for which no native application exists.

Repository: [calaos-web-app](https://github.com/calaos/calaos-web-app), written in **JavaScript**.

Like the other interfaces, it is only an **API client**: it never talks to the hardware directly, and everything it displays comes from `calaos_server`. See [Calaos Server]({{% relref "dev/calaos_server" %}}).

## Building

The toolchain relies on Node.js, with Bower for browser-side dependencies and Gulp as the build tool.

```sh
npm install -g bower gulp
npm install
bower install
gulp
```

To develop with live reload:

```sh
gulp serve
```

In that mode the application does not run on the Calaos server, so you must tell it where to reach it, by setting the API URL in `src/scripts/dev_config.js`, in the form `ws://server:5454/api`.

{{% notice note %}}
This build chain (Bower, Gulp) dates from the early days of the project and is not what one would choose today. It is worth knowing before setting up a development environment: recent Node.js versions may require a few adjustments.
{{% /notice %}}

## Deployment

On a Calaos OS installation, the WebApp is served alongside the rest of the system; incoming connections are routed by HAProxy, which also handles the certificate set up by [dynamic DNS]({{% relref "dev/calaos_ddns" %}}).

## Writing your own client

The WebApp is a good starting point for understanding how to talk to the server from a browser: opening the WebSocket, `login`, `get_home`, then handling the incoming events.

The complete sequence and the list of methods are described in [Calaos Server]({{% relref "dev/calaos_server" %}}).
