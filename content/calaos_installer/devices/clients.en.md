+++
title = "Display and control"
date = 2026-10-15T09:25:00Z
weight = 40
summary = "Climates in the web app, the mobile app, the wall panels and the MCP assistant."
+++

All clients read the same device information and follow the same principles:

- the main screen shows the **setpoint**, the **room temperature**, the **mode** and a **status**;
- the mode is a pill: a tap opens the mode choice;
- fan, swing and humidity are under **More settings** and only appear if the device offers them;
- the dial is an almost closed arc; in `auto` mode it carries **two handles** and two pairs of − / + buttons;
- when the two setpoints are at the minimum gap, the inner buttons are refused and the screen says "Minimum gap 2°".

Colours have a meaning: **yellow** when the unit is heating, **cyan** when cooling, **amber** when waiting, **red** when offline; a stopped, unknown or idle unit stays neutral.

## Displayed status

| Status | Meaning |
|---|---|
| Heating, Cooling, Drying, Fan | the current action reported by the unit or the thermostat |
| Idle | the unit is on but doing nothing for now |
| Starts in N min | a wanted output waits for the end of its minimum off time |
| Offline | the relayed unit is unreachable (ESPHome disconnected) |
| Sensor silent, Sensor disconnected, Waiting for the sensor, Sensor missing, Sensor out of range | thermostat safety: see [Internal thermostat]({{% relref "calaos_installer/devices/thermostat" %}}) |
| Humidity too high, Humidity sensor silent | floor cooling cut (dew point) |
| Stopped | `off` mode |
| Unknown mode | the device has not received any mode yet: see [Troubleshooting]({{% relref "calaos_installer/devices/troubleshooting" %}}) |

## Web app

Climates are **rows** in the room list: a mini dial, the name, the status, the setpoint, the room temperature.

![Climate list of the room in the web app](/en/calaos_installer/devices/images/web-room-list.png?width=45pc&classes=shadow)
*One row per device: here a unit heating, a floor cooling, a heat pump idle, a radiator waiting, a stopped unit and an offline unit.*

A click on a row opens the **settings panel** on the right of the screen (below 1024 px wide, a centred window). You set the setpoint with the − / + buttons, and see the status and the room temperature.

![Settings panel in auto mode](/en/calaos_installer/devices/images/web-settings-panel.png?width=45pc&classes=shadow)
*A unit in auto mode: two setpoints, 19.5° and 24.5°.*

![Settings panel of a single-setpoint unit](/en/calaos_installer/devices/images/web-settings-single.png?width=45pc&classes=shadow)
*A single-setpoint unit, in Cool mode.*

The mode pill opens the mode choice, and **More settings** opens fan, swing and humidity.

![Mode choice](/en/calaos_installer/devices/images/web-mode-choice.png?width=45pc&classes=shadow)
*The mode choice: only the modes the device offers appear.*

![More settings](/en/calaos_installer/devices/images/web-more-settings.png?width=45pc&classes=shadow)
*More settings: fan, swing, humidity.*

An internal thermostat looks the same; only the modes it has are offered.

![Floor thermostat](/en/calaos_installer/devices/images/web-thermostat.png?width=45pc&classes=shadow)
*The lounge floor, cooling.*

The minimum gap between the two setpoints is enforced on screen:

![Minimum gap](/en/calaos_installer/devices/images/web-range-stop.png?width=45pc&classes=shadow)
*The inner buttons are refused when the minimum gap is reached.*

A safety fault shows in the row and in the panel, in red:

![Sensor fault](/en/calaos_installer/devices/images/web-fault.png?width=45pc&classes=shadow)
*A radiator whose sensor is silent: heating is cut, the setpoint stays adjustable.*

## Mobile app

On a phone the list shows for each climate the name (yellow when the unit is active), the room temperature, the mode pill, the status and the setpoint ("19.5 – 24.5°" in `auto`).

![Climate list on mobile](/en/calaos_installer/devices/images/mobile-list.png?width=18pc&classes=shadow)
*The list on a phone: one climate per status.*

A tap opens a **settings sheet** at the bottom of the screen (on a computer, a dialog).

![Settings on mobile](/en/calaos_installer/devices/images/mobile-sheet.png?width=18pc&classes=shadow)
*Settings of a unit in auto mode.*

![Mode choice on mobile](/en/calaos_installer/devices/images/mobile-sheet-mode.png?width=18pc&classes=shadow)
*The mode choice, on top of the settings.*

![More settings on mobile](/en/calaos_installer/devices/images/mobile-sheet-more.png?width=18pc&classes=shadow)
*More settings.*

![Thermostat settings on mobile](/en/calaos_installer/devices/images/mobile-sheet-thermostat.png?width=18pc&classes=shadow)
*The lounge floor.*

On a computer, the same list and a dialog in place of the bottom sheet:

![List on desktop](/en/calaos_installer/devices/images/mobile-desktop-list.png?width=22pc&classes=shadow)
*The list, desktop version.*

![Settings dialog on desktop](/en/calaos_installer/devices/images/mobile-desktop-dialog.png?width=26pc&classes=shadow)
*The settings dialog.*

## Wall panel

On the wall panel (see [Remote UI]({{% relref "hardware/remote_ui" %}})), the content depends on the **size of the widget** in the grid. In Calaos Installer, add a widget of type **Climate** and link it to the device.

**Small widgets** (1×1, 2×1, 1×2) show the room temperature, the setpoint, the icon and status word, and the name. There are no − / + buttons: a tap opens the **full-screen view**.

![Small climate widgets on the wall panel](/en/calaos_installer/devices/images/rui-small-widgets.png?width=30pc&classes=shadow)
*Several small widgets on a wall panel page.*

![Full-screen view](/en/calaos_installer/devices/images/rui-fullscreen.png?width=30pc&classes=shadow)
*The full-screen view: dial, setpoints, mode pill, More settings.*

The mode and the settings open in stacked popups:

![Mode popup](/en/calaos_installer/devices/images/rui-mode-popup.png?width=30pc&classes=shadow)
*The mode choice.*

![More settings popup](/en/calaos_installer/devices/images/rui-more-popup.png?width=30pc&classes=shadow)
*More settings.*

A **large widget** (2×2, 3×3 and up) holds the whole climate, with no intermediate view: mode pill and `⋯` at the top, dial and − / + buttons in the middle, the name at the bottom like other widgets.

![2×2 widget](/en/calaos_installer/devices/images/rui-large-2x2.png?width=30pc&classes=shadow)
*A 2×2 widget.*

![3×3 widget](/en/calaos_installer/devices/images/rui-large-3x3.png?width=30pc&classes=shadow)
*A 3×3 widget in auto mode.*

![800×480 screen](/en/calaos_installer/devices/images/rui-800x480.png?width=30pc&classes=shadow)
*On an 800×480 panel.*

{{% notice tip %}}
If a widget linked to a climate shows a red "Unsupported", it is a widget type the panel software does not know: see [Troubleshooting]({{% relref "calaos_installer/devices/troubleshooting" %}}).
{{% /notice %}}

## The MCP assistant

The Calaos MCP assistant (a sidecar of [Calaos Server]({{% relref "dev/calaos_server" %}})) sees a device as an ordinary IO whose value is the mode. It can command it with `set_io_state`, in simple mode (`off`) or in JSON (`{"mode":"cool","target_temp":24}`); the tool description lists the fields of each device. It can also read the whole device object (capabilities, state). A thermostat's **relays** are hidden and cannot be written: the assistant sets the thermostat, never its outputs.
