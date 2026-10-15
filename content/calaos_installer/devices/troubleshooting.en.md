+++
title = "Troubleshooting"
date = 2026-10-15T09:35:00Z
weight = 60
summary = "\"Unknown\", \"Offline\", \"Unsupported\" widget, invisible device: causes and diagnostic commands."
+++

## The three diagnostic commands

**What the module publishes over MQTT** (from a computer with `mosquitto-clients`, or with MQTTX):

```sh
mosquitto_sub -h 192.168.1.10 -p 1883 -u user -P password -v -t 'clim_salon/#'
```

**What the Calaos server says** (over SSH on the box, see [SSH access]({{% relref "calaos_os/advanced/shell" %}})):

```sh
journalctl -u calaos-server -f
journalctl -u calaos-server --since "1 hour ago" | grep -i device
```

Messages about devices (link not found, command refused, output already taken, write on a protected output) contain the word `device`.

**What the server sends to the interfaces**: the `get_home` answer contains, for each device, the `device` object (capabilities, state, linked IOs). With `curl` and `jq`:

```sh
curl -s -X POST http://192.168.1.10:5454/api.php \
  -d '{"action":"get_home","cn_user":"user","cn_pass":"pass"}' \
  | jq '.. | objects | select(.gui_type? == "climate")'
```

A device missing from this output is not loaded or not visible (see below). A device's `state` key gives the mode; `device.state` gives the temperature, setpoints, action, and `connected`, `fault` or `lockout` when they apply.

## "Unknown mode": the device received no mode

The device reads its mode on the topic `<prefix>/climate/<id>/mode/state`. Nothing arrives:

1. Listen with `mosquitto_sub -v -t '<prefix>/#'`. If the module publishes nothing, the problem is on the ESPHome side (Wi-Fi, broker, `topic_prefix`).
2. If the module publishes, compare the **prefix** and the entity **id** with the device's (device editor, "mode" IO, `topic_sub` field). A typo or confusing the module name with the entity name is the most frequent cause: see [ESPHome air conditioner]({{% relref "hardware/esphome_climate" %}}).
3. Check "Two setpoints": if the module publishes `target_temperature_low/state` and `target_temperature_high/state`, the device must have two setpoints; if it publishes `target_temperature/state`, one.
4. Check that the device's broker, port, user and password are the module's.
5. Restart the module so that it republishes its state.

## "Offline"

The device is "offline" when the `<prefix>/status` topic is not `online`. ESPHome publishes it on connection and switches it to `offline` on disconnection (last will message). Check with `mosquitto_sub -v -t '<prefix>/status'`: module off, lost on Wi-Fi, or a different `topic_prefix` than the device's.

## "Sensor silent", "Sensor disconnected"... on a thermostat

These are safeties, not thermostat failures: see the table in [Internal thermostat]({{% relref "calaos_installer/devices/thermostat" %}}). Check the linked sensor (value, connection); as soon as it delivers a valid reading, regulation resumes by itself.

## The thermostat does not heat

1. The displayed status gives the reason: "Starts in N min" (minimum off time running), a safety, "Idle" (the temperature is within the band, which is normal), "Stopped" (`off` mode).
2. Look at the thermostat's **action** IO: `heating`, `idle` or `off`.
3. If the action is `heating` but the relay does not move, the problem is downstream: the relay or its driver (Wago...).
4. If the relay turns off right after being turned on, a rule or script also writes it: the log contains "protected output" warnings. Command the mode, not the relay.

## The unit does not follow the room temperature

The `<prefix>/calaos/remote_temp` topic must receive a value every 120 s:

```sh
mosquitto_sub -h 192.168.1.10 -v -t 'clim_salon/calaos/remote_temp'
```

- Nothing: the room sensor has read nothing since startup, has been silent for `sensor_timeout`, or the "Room sensor" link is missing. Calaos never relays a stale value.
- Values arrive but the unit ignores them: check in the YAML the `mqtt_subscribe` sensor topic, `remote_temperature_source` and `remote_temperature_timeout` (longer than 120 s).

To test the ESPHome side without Calaos: `mosquitto_pub -h ... -t 'clim_salon/calaos/remote_temp' -m 21.5`.

## A command does nothing or goes back

A setpoint outside the bounds is clamped. A field or value the device does not offer (a fan mode missing from the `fan_modes` parameter, `cool` without a cooling output) is **refused and logged**: look for the line in `journalctl -u calaos-server`. A gap between low and high setpoint that is too small (`min_range`) is refused the same way.

## The wall panel widget shows "Unsupported"

The widget type written by Calaos Installer is not known to the panel. Update Calaos Installer, open the panel page in the Remote UI editor (the widget takes the **Climate** type), send the configuration, then update the panel software if the problem persists: a panel too old has no Climate widget.

## The device does not appear in the web app, mobile app or Calaos Home

1. Is the server up to date? An old server did not write the `visible` attribute of a device, which the apps require. Without an update, add `visible="true"` to the `<calaos:device>` element.
2. Is the device in `get_home` (`curl` command above)? If not, look at the log when the server starts: a malformed file or a load error is reported there.
3. Was it sent? The device only exists on the server after the [project upload]({{% relref "getting_started/upload" %}}).

## Calaos Installer refuses to send or open the project

- "this io.xml would drop N device(s)": the server protects its devices from a Calaos Installer that is too old. Update it.
- The project opens read-only: it was saved by a newer version of the format. Update Calaos Installer.
