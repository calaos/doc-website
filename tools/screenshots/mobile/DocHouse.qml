import QtQuick

// The climate devices of the demo house (tools/screenshots/DEMO_HOUSE.md) as the server publishes them.
QtObject {
    function caps(over) {
        var c = {
            modes: ["off", "heat", "cool", "auto", "dry", "fan_only"],
            target_temp: { min: 17, max: 28, step: 0.5, unit: "°C", min_range: 1 },
            target_temp_range: true,
            fan_modes: ["auto", "quiet", "low", "medium", "high"],
            swing_modes: ["off", "vertical", "horizontal", "both"],
            reports_action: true, humidity: true, remote_temp: true
        }
        for (var k in over)
            c[k] = over[k]
        return c
    }

    readonly property var thermostatCaps: ({
        modes: ["off", "heat", "cool", "auto"],
        target_temp: { min: 16, max: 28, step: 0.5, unit: "°C", min_range: 2 },
        target_temp_range: true, reports_action: true, humidity: true
    })

    readonly property var house: ({
        salon: { name: "Clim salon", impl: "proxy", caps: caps({}),
                 st: { mode: "auto", current_temp: 19.2, target_temp: 22, target_temp_low: 19.5, target_temp_high: 24.5, action: "heating", fan_mode: "auto", swing_mode: "off", humidity: 48 } },
        chambre: { name: "Clim chambre", impl: "proxy",
                   caps: caps({ modes: ["off", "heat", "cool", "auto"], target_temp: { min: 16, max: 30, step: 1, unit: "°C" }, target_temp_range: false,
                                fan_modes: ["auto", "low", "medium", "high"], swing_modes: [], humidity: false, remote_temp: false }),
                   st: { mode: "off", target_temp: 24, action: "off", fan_mode: "low" } },
        bureau: { name: "Radiateur bureau", impl: "thermostat",
                  caps: { modes: ["off", "heat"], target_temp: { min: 16, max: 28, step: 0.5, unit: "°C" }, reports_action: true },
                  st: { mode: "heat", current_temp: 21, target_temp: 21, action: "idle" } },
        pac: { name: "PAC cuisine", impl: "thermostat",
               caps: { modes: ["off", "heat", "cool", "auto"], target_temp: { min: 16, max: 28, step: 0.5, unit: "°C", min_range: 2 }, target_temp_range: true, reports_action: true },
               st: { mode: "auto", current_temp: 22.1, target_temp: 22, target_temp_low: 20, target_temp_high: 25, action: "idle", lockout: { output: "cool", remaining: 140 } } },
        plancher: { name: "Plancher séjour", impl: "thermostat", caps: thermostatCaps,
                    st: { mode: "cool", current_temp: 24.6, target_temp: 23, target_temp_low: 20, target_temp_high: 25, action: "cooling", humidity: 52 } },
        buanderie: { name: "Clim buanderie", impl: "proxy",
                     caps: caps({ modes: ["off", "heat"], target_temp_range: false, fan_modes: [], swing_modes: [], humidity: false }),
                     st: { mode: "heat", current_temp: 17.5, target_temp: 20, connected: false } }
    })

    function ioOf(key) {
        var m = house[key]
        return harness.makeClimate({ id: "io_" + key, name: m.name, gui_type: "climate", state: m.st.mode, rw: "true",
            device: { family: "climate", impl: m.impl, capabilities: m.caps, state: m.st,
                      ios: { mode: "m", target_temp: "t", target_temp_low: "l", target_temp_high: "h" } } })
    }

    function find(item, name) {
        if (item === null || item === undefined)
            return null
        if (item.objectName === name)
            return item
        for (var i = 0; i < item.children.length; i++) {
            var r = find(item.children[i], name)
            if (r)
                return r
        }
        return null
    }
}
