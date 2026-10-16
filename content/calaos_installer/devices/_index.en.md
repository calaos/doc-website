+++
title = "Devices: heating and air conditioning"
date = 2026-10-15T09:00:00Z
weight = 65
summary = "Air conditioners, heat pumps, radiators, floor heating: create and control a climate device."
+++

Calaos drives heating and air conditioning in two ways, which look the same in every interface (web app, mobile app, wall panels) and in rules:

- **an air conditioner or heat pump that regulates by itself** (for example a Mitsubishi or Fujitsu split driven by an ESPHome module): Calaos shows its state, sends setpoints, and can relay the temperature of a room sensor to it. This is the `proxy` kind;
- **a thermostat that Calaos runs itself**: Calaos reads a temperature sensor, compares it to the setpoint, and switches one or two relays (radiators, floor heating, valves, a heat pump on dry contacts). This is the `thermostat` kind.

Either way, the equipment is a **device**: one object that groups all the IOs of one appliance, instead of twenty MQTT IOs created by hand.

## Where to start

| You want to... | Page |
|---|---|
| understand what a device is | [How a device works]({{% relref "calaos_installer/devices/concepts" %}}) |
| add an ESPHome air conditioner | [ESPHome air conditioner]({{% relref "hardware/esphome_climate" %}}) |
| let Calaos regulate with relays | [Internal thermostat]({{% relref "calaos_installer/devices/thermostat" %}}) |
| create or edit a device in the installer | [Create a device with the wizard]({{% relref "calaos_installer/devices/templates" %}}) |
| see the result on your screens | [Display and control]({{% relref "calaos_installer/devices/clients" %}}) |
| start from a complete project | [Example projects]({{% relref "calaos_installer/devices/examples" %}}) |
| schedule the heating | [Heating rules]({{% relref "calaos_installer/rules_heating" %}}) |
| understand a problem | [Troubleshooting]({{% relref "calaos_installer/devices/troubleshooting" %}}) |

{{% children description="true" %}}
