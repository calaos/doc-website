+++
title = 'Music zones'
date = 2024-03-06T20:05:27Z
weight = 100
summary = 'Music zones (Squeezebox)'
+++

## Introduction

A **music zone** is an audio player installed in a room, which Calaos drives like the rest of the house: starting a playlist, setting the volume, skipping to the next track, stopping everything when you leave.

Each equipped room becomes an independent zone, with its own playback and its own volume.

## Supported systems

| System | Description |
|---|---|
| **Squeezebox** | Logitech Squeezebox players and compatibles, driven by a Lyrion / Logitech Media Server |
| **Roon** | Players managed by a Roon server |

In both cases Calaos does not play the music itself: it **controls the playback system** already in place, and gets back the playback state and information about the current track.

## What Calaos displays

Once the zone is declared, the interfaces present a complete player: current track, album art, playback and volume controls, access to playlists, artists, albums and radios.

This library comes from the music server, not from Calaos: the tracks you see are the ones it knows about.

## Prerequisites

**The music server must be installed and working**, with its players already declared and audible. Calaos sits on top of a working installation, it does not replace it.

Before anything else, check that you can start a track in a room from the system's native interface. If you cannot, the problem is upstream of Calaos.

{{% notice tip %}}
Give the music server a **fixed IP address**, as well as the players. This is the most frequent cause of zones disappearing after a power cut.
{{% /notice %}}

## Adding a zone

In Calaos Installer, **Add → Music zone**, then state the system and the player concerned.

Calaos Installer can **detect Squeezebox players** present on the network, which saves entering addresses by hand.

## Using it in rules

A music zone is used like any IO:

- **stop the music everywhere** in the "Leaving" scenario;
- **start a playlist** at wake-up time, with a schedule;
- **lower the volume** when the doorbell rings;
- **stop playback** in a room when it empties, from a presence detector.

See [Create rules]({{% relref "calaos_installer/rules" %}}), [Scenario]({{% relref "calaos_installer/scenario" %}}) and [Time scheduling]({{% relref "calaos_installer/time" %}}).

## Diagnosis

If a zone does not answer:

1. **does the player work** from the music system's native interface?
2. **is the music server reachable** from the Calaos server?
3. **have the addresses changed** since it was configured?
4. **is the player switched on?** Some players shut down completely and stop answering.

See [Logs]({{% relref "calaos_os/configuration/logs" %}}).

## See also

To drive a home cinema receiver rather than a multi-room player, see [Audio/Video Receivers]({{% relref "hardware/avr" %}}).
