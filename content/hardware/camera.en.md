+++
title = 'Camera'
date = 2024-03-06T20:05:18Z
weight = 90
summary = 'MJPEG Camera'
+++

## Introduction

Calaos displays the pictures from your IP cameras in its interfaces: on the touchscreen, in the mobile applications and in the web app. A camera picture can also be **attached to an email** sent by a rule, so you receive a snapshot at the moment of an event.

The stream used is **MJPEG**, a simple format that virtually every IP camera can produce.

## Supported models

Calaos knows several camera families directly, and knows how to ask them for a picture:

| Family | Note |
|---|---|
| **Standard MJPEG** | Works with any camera exposing an MJPEG stream |
| **Axis** | — |
| **Foscam** | — |
| **Gadspot** | — |
| **Planet** | — |
| **Synology Surveillance Station** | Goes through the surveillance server rather than the camera |
| **Reolink** | See the dedicated section below |

{{% notice tip %}}
If your model is not in the list, try the **standard MJPEG** type: all you need is the stream address, which the manufacturer usually documents. Many unlisted cameras work this way.
{{% /notice %}}

## Adding a camera

In Calaos Installer, **Add → Camera**, then choose the family matching your model. Fill in its network address and, if it asks for them, its credentials.

{{% notice warning %}}
**Change the default password on your cameras.** They are the first device targeted on a home network, and a camera left on its factory credentials is accessible to anyone who reaches your network.

Do not expose it directly on the Internet — see [Dynamic DNS]({{% relref "calaos_os/configuration/calaos_ddns" %}}) for controlled remote access.
{{% /notice %}}

## Adjusting the picture

Two parameters let you adapt the picture to the display, without touching the camera's own settings:

| Name | Type | Required | Description |
|---|---|---|---|
| `rotate` | int | no | Rotation of the picture in degrees. For example `90` for clockwise, `-90` for counter-clockwise |
| `width` | int | no | Desired width in pixels. The picture is resized keeping its proportions. Leave empty to keep the original size |

`width` is useful on wall screens: shrinking a 4K camera picture to the width actually displayed noticeably lightens both the network and the display.

## Reolink event detection

**Reolink** cameras go further than the picture alone: they can report what they detect. Calaos can turn these events into an input, usable like any switch in your rules.

| Name | Type | Required | Description |
|---|---|---|---|
| `hostname` | string | yes | IP address or name of the camera |
| `username` | string | yes | Login for the camera |
| `password` | string | yes | Password |
| `event_type` | list | yes | Type of event to listen for. Defaults to `motion` |

The available event types:

| Value | Detection |
|---|---|
| `motion` | Motion |
| `person` | Person |
| `vehicle` | Vehicle |
| `pet` | Pet |
| `cry` | Crying |
| `face` | Face |
| `package` | Package |
| `visitor` | Visitor / doorbell |

{{% notice tip %}}
These specialised detections are far better than plain motion: a rule triggered on `person` ignores branches moving in the wind and the neighbourhood cats, where `motion` will send you notifications all night.
{{% /notice %}}

A Reolink input is then used like a conventional detector: switching on outdoor lighting when someone arrives, sending a notification for the doorbell, recording the event in the history. See [Create rules]({{% relref "calaos_installer/rules" %}}).

## Sending a picture by email

An email action can attach a camera picture at the moment the rule triggers. It is the simplest way to receive a snapshot on detection.

Email sending must be configured beforehand: see [Email]({{% relref "calaos_os/configuration/email" %}}).

## Diagnosis

If the picture does not appear:

1. **is the camera reachable** from the Calaos server, at the given address?
2. **does the stream open** in a browser, using the full MJPEG stream address?
3. **are the credentials** correct? Many cameras silently refuse a badly authenticated request.
4. **does the selected model** match? If in doubt, try the standard MJPEG type.

See [Logs]({{% relref "calaos_os/configuration/logs" %}}) for server-side errors.
