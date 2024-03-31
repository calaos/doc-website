+++
title = 'KNX using Wago'
date = 2024-03-28T14:23:16Z
+++

## KNX Installation

It is possible to use the Wago controller to manage a KNX installation and act as a KNX gateway.

KNX is a standardized communication protocol for smart buildings. KNX emerged from the merger of the EHS, EIB (or Instabus), and Bâtibus standards.

Unlike a standard electrical installation, there is no wired connection between the control units and the power supply: for example, a switch is not directly connected to the corresponding lamp. Indeed, devices and electrical equipment are connected via the BUS, which operates at 28 V. All BUS devices can be programmed using a common tool. Thus, the KNX BUS allows for a simple and highly flexible installation. Furthermore, subsequent modifications can be made without touching the wiring.

One of the advantages of using the Wago controller with KNX is the ability to have a degraded mode where the controller can turn on a light using a relay with a KNX push-button, for example.

## Prerequisites

The Calaos system requires several components to work with the KNX bus:

- A Wago station head 750-849
- A Wago terminal 753-646
- A 30Vdc KNX power supply to power the KNX bus
- KNX modules
- ETS4 / ETS5 software for configuration

## Architecture

The KNX bus communicates over a twisted pair (TP) link.

However, it is possible to communicate over other media:

- Through power line (PL)
- Through radio frequency (RF)
- Through infrared
- Through Ethernet (KNX over IP)

Each element connected to the KNX bus is independent of the others. It can send information that will be received by other elements but processed only by the relevant element.

![pic](/en/hardware/wago/images/bus_knx_.png?width=20pc&classes=shadow)

PL: Power Line TP: Twisted Pair

### Power Supply

The bus must be powered with a nominal DC voltage of 29V. Most components are powered directly by the bus. The lower limit of the supply voltage is 21V. The average consumption of a component is around 10 mA.

### Bus Limits

Like all buses, KNX has cable length limits to respect.

- Maximum distance between two modules: 700 m
- Maximum distance between a module and its power supply: 350 m
- Maximum length of the bus: 1000 m
- Maximum distance between two power supplies: 200 m

![pic](/en/hardware/wago/images/wago.png?width=20pc&classes=shadow)

## Topologies

An installation can be divided into lines and zones. A line contains a maximum of **64 participants** (modules). A zone has a maximum of **15 lines** connected to the main line by line couplers. A backbone connects a maximum of **15 zones** through zone couplers.

![pic](/en/hardware/wago/images/topologie.png?width=20pc&classes=shadow)

## Programming

TODO!
