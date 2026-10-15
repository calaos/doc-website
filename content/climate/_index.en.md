+++
title = "Heating and air conditioning"
date = 2026-10-15T09:00:00Z
weight = 25
chapter = true
+++

### Driving comfort

# Heating and air conditioning

Calaos drives heating and air conditioning in two ways, which look the same in every interface (web app, mobile app, wall panels) and in rules:

- **an air conditioner or heat pump that regulates by itself** (for example a Mitsubishi or Fujitsu split driven by an ESPHome module): Calaos shows its state, sends setpoints, and can relay the temperature of a room sensor to it. This is the `proxy` kind;
- **a thermostat that Calaos runs itself**: Calaos reads a temperature sensor, compares it to the setpoint, and switches one or two relays (radiators, floor heating, valves, a heat pump on dry contacts). This is the `thermostat` kind.

Either way, the equipment is a **device**: one object that groups all the IOs of one appliance, instead of twenty MQTT IOs created by hand.

## Where to start

| You want to... | Page |
|---|---|
| understand what a device is | [How a device works]({{% relref "climate/concepts" %}}) |
| add an ESPHome air conditioner | [ESPHome air conditioner]({{% relref "climate/esphome" %}}) |
| let Calaos regulate with relays | [Internal thermostat]({{% relref "climate/thermostat" %}}) |
| create or edit a device in the installer | [Create a device with the wizard]({{% relref "climate/templates" %}}) |
| see the result on your screens | [Display and control]({{% relref "climate/clients" %}}) |
| schedule the heating | [Heating rules]({{% relref "climate/rules" %}}) |
| understand a problem | [Troubleshooting]({{% relref "climate/troubleshooting" %}}) |

{{% notice note %}}
The screenshots in this documentation come from an invented demo house (living room, bedroom, office, kitchen, lounge). Names, addresses and values are not real. The interface is shown in French.
{{% /notice %}}

{{% notice info %}}
Devices need an up-to-date Calaos server, web app, mobile app, wall panel and Calaos Installer. An older Calaos Installer refuses to open a project that contains devices, rather than deleting them silently.
{{% /notice %}}

{{% children description="true" %}}
