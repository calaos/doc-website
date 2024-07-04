+++
title = 'MQTT'
date = 2024-03-06T20:04:25Z
weight = 30
summary = 'Devices using MQTT protocol'
+++

Here's the complete document translated into English:

## Introduction to MQTT

MQTT (Message Queuing Telemetry Transport) is a lightweight messaging protocol ideal for devices with limited resources and low-bandwidth networks. MQTT operates on a publish/subscribe model, where clients can publish messages to specific "topics" and subscribe to these topics to receive messages.

### What is an MQTT Broker?

An MQTT broker is a server that receives messages published by clients and distributes them to the corresponding subscribers. Mosquitto is a popular example of an MQTT broker that can be used with Calaos.

### Enabling Mosquitto in Calaos

Mosquitto is already integrated into Calaos OS. You just need to enable it to use it.

To enable Mosquitto in Calaos, use the following command:
```bash
systemctl enable --now mosquitto
```
You can configure authentication and other settings in the configuration file located at `/mnt/calaos/mosquitto/config/mosquitto.conf`.

To apply the configuration changes, restart Mosquitto with:
```bash
systemctl restart mosquitto
```

## Debugging

To debug what is happening with MQTT, you can use a tool like MQTTX. MQTTX allows you to connect to the MQTT broker and monitor the exchanged messages, which is useful for testing and verifying the configurations of topics and payloads.

## Using MQTT in Calaos

In Calaos, MQTT can be used for different types of inputs and outputs (IO). Each MQTT IO must be configured with `topic_sub`, `topic_pub`, and `path`.

### Supported MQTT IO Types

- Temperature sensor
- Switch
- Analog input
- Text input
- Light On/Off
- Dimmable light
- RGB light
- Analog output

## Example Configuration of an MQTT Temperature Sensor

To configure an MQTT temperature sensor in Calaos, suppose the sensor sends its value in JSON format to the topic `mqtt_devices/temp_01` with the following JSON payload: `{ "temp": 24.15, "humidity": 43, "battery": 85 }`. For this configuration, we will focus on the temperature value.

### Configuration Parameters:

Here is the configuration to adopt in Calaos Installer when creating the MQTT Temperature IO:

| Name        | Value                  |
|-------------|------------------------|
| name        | `Temperature Sensor`   |
| topic_sub   | `mqtt_devices/temp_01` |
| path        | `temp`                 |
| host        | `127.0.0.1`            |
| port        | `1883`                 |
| precision   | `1`                    |

By configuring these parameters in Calaos, you will be able to receive and display the temperature values sent by the MQTT temperature sensor.

### General Parameters for MQTT IOs

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | yes         | Topic to subscribe to.                                                                                                                      |
| topic_pub   | string | yes         | Topic to publish to.                                                                                                                        |
| path        | string | yes         | Path to find the value in the MQTT payload. If the payload is JSON, the information will be extracted based on the path.                |
| user        | string | no          | User for authentication with the MQTT broker. Password must be defined in this case.                                            |
| keepalive   | int    | no          | Keepalive time in seconds. Time between two MQTT PINGs.                                                                                       |
| port        | int    | no          | TCP port of the MQTT broker. Default value is 1883.                                                                                           |
| password    | string | no          | Password for authentication with the MQTT broker. User must be defined in this case.                                             |
| host        | string | no          | IP address of the MQTT broker. Default value is 127.0.0.1.                                                                                    |
| enabled     | bool   | no          | Enable the Input/Output. Default value is true. This parameter is added if it is not found in the configuration.                        |
| log_history | bool   | no          | If enabled, write an entry in the event log history for this IO.                                                                        |
| visible     | bool   | no          | Display the Input/Output on all user interfaces if set. Default to true.                                                       |
| logged      | bool   | no          | If enabled, and if influxdb is enabled in the local configuration, send the value to influxdb for this IO.                                      |
| gui_type    | string | no          | Internal graphical type for all Calaos objects. Read-only parameter, set automatically.                                           |
| io_type     | string | yes         | IO type, can be "input", "output", "inout".                                                                                                  |
| id          | string | yes         | Unique ID identifying the Input/Output in calaos-server.                                                                                        |

### Specific Parameters for Each Type of MQTT IO

#### MqttInputAnalog

Analog input used to read analog values, display them, and use them in rules.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | yes         | Topic to subscribe to.                                                                                                                      |
| topic_pub   | string | yes         | Topic to publish to.                                                                                                                        |
| precision   | int    | no          | Precision of the returned value. Represents the number of decimals after the dot.                                                            |
| name        | string | yes         | Name of the Input/Output.                                                                                                                          |
| path        | string | yes         | Path to find the value in the MQTT payload.                                                                                                |
| io_style    | list   | yes         | GUI display style.                                                                                                    |
| coeff_a     | float  | no          | Used with coeff_b to apply an equation of the form `displayed_value = coeff_a * raw_value + coeff_b`. Default value is 1.0.  |
| coeff_b     | float  | no          | Used with coeff_a to apply an equation of the form `displayed_value = coeff_a * raw_value + coeff_b`. Default value is 0.0.  |
| unit        | string | no          | Unit displayed on the UI as a suffix.                                                                                  |

#### MqttInputString

Text input used to read string values.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | yes         | Topic to subscribe to.                                                                                                                      |
| topic_pub   | string | yes         | Topic to publish to.                                                                                                                        |
| name        | string | yes         | Name of the Input/Output.                                                                                                                          |
| path        | string | yes         | Path to find the value in the MQTT payload.                                                                                                |

#### MqttInputSwitch

Switch with press/release states.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| on_value    | string | yes         | Value to interpret as ON.                                                                                                                   |
| off_value   | string | yes         | Value to interpret as OFF.                                                                                                                  |

#### MqttInputTemp

Temperature sensor used to display temperature and control heating devices via rules based on the temperature value.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | yes         | Topic to subscribe to.                                                                                                                      |
| topic_pub   | string | yes         | Topic to publish to.                                                                                                                        |
| precision   | int    | no          | Precision of the returned value.                                                                                                                |
| name        | string | yes         | Name of the Input/Output.                                                                                                                          |
| path        | string | yes         | Path to find the value in the MQTT payload.                                                                                                |

#### MqttOutputAnalog

Analog output used to control analog output devices connected to Calaos.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| data        | string | yes         | Data sent when publishing to the topic.                                                                                       |
| step        | float  | no          | Set a step for increment/decrement value. Default is 1.0.                                                                 |

#### MqttOutputLight

Basic light with only two states, ON or OFF. Can also be used to control simple relays.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| data        | string | yes         | Data sent when publishing to the topic.                                                                                       |
| on_value    | string | yes         | Value to interpret as ON.                                                                                                                   |
| off_value   | string | yes         | Value to interpret as OFF.                                                                                                                  |

#### MqttOutputLightDimmer

Light with dimming control. Light intensity can be changed.

| Name        | Type   | Mandatory | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| data        | string | yes         | Data sent when publishing to the topic.                                                                                       |

#### MqttOutputLightRGB

RGB light. Choose a color to be set for this light.

| Name              | Type   | Mandatory | Description                                                                                                                                      |
|------------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| path_brightness  | string | yes         | Path to find the brightness value in the MQTT payload.                                                                                 |
| path_y           | string | yes         | Path to find the Y (X/Y Color space) value in the MQTT payload.                                                                         |
| path_x           | string | yes         | Path to find the X (X/Y Color space) value in the MQTT payload.                                                                         |
| data             | string | yes         | Data sent when publishing the color to the topic.                                                                          |
