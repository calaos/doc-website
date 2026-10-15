#!/usr/bin/env python3
"""Scenarios of the climate documentation screenshots (demo house, see ../DEMO_HOUSE.md).

Usage: python3 -I gen_scenarios.py OUTDIR
"""
import copy
import json
import os
import sys

out = sys.argv[1]
os.makedirs(out, exist_ok=True)


def caps(modes, lo, hi, step, rng=True, fan=None, swing=None):
    c = {"modes": modes,
         "target_temp": {"min": lo, "max": hi, "step": step, "unit": "°C", "min_range": 2},
         "target_temp_range": rng, "reports_action": True}
    if fan:
        c["fan_modes"] = fan
    if swing:
        c["swing_modes"] = swing
    return c


def dev(name, impl, mode, c, state):
    state = dict(state)
    state["mode"] = mode
    return {"state": mode, "gui_type": "climate", "name": name,
            "device": {"family": "climate", "impl": impl, "capabilities": c, "state": state,
                       "ios": {"mode": "m", "target_temp": "t"}}}


ALL = ["off", "heat", "cool", "auto", "dry", "fan_only"]
D = {
    "salon": dev("Clim salon", "proxy", "auto",
                 caps(ALL, 17, 28, 0.5, fan=["auto", "quiet", "low", "medium", "high"],
                      swing=["off", "vertical", "horizontal", "both"]),
                 {"current_temp": 19.2, "target_temp": 22.0, "target_temp_low": 19.5, "target_temp_high": 24.5,
                  "action": "heating", "fan_mode": "auto", "swing_mode": "off", "humidity": 48}),
    "chambre": dev("Clim chambre", "proxy", "cool",
                   caps(["off", "heat", "cool", "auto"], 16, 30, 1, fan=["auto", "low", "medium", "high"]),
                   {"target_temp": 24.0, "fan_mode": "low"}),
    "bureau": dev("Radiateur bureau", "thermostat", "heat",
                  caps(["off", "heat"], 7, 30, 0.5, rng=False),
                  {"current_temp": 19.4, "target_temp": 21.0, "action": "heating"}),
    "cuisine": dev("PAC cuisine", "thermostat", "auto",
                   caps(["off", "heat", "cool", "auto"], 7, 30, 0.5),
                   {"current_temp": 22.1, "target_temp": 22.5, "target_temp_low": 20.0, "target_temp_high": 25.0,
                    "action": "idle", "lockout": {"output": "cool", "remaining": 140}}),
    "sejour": dev("Plancher séjour", "thermostat", "cool",
                  caps(["off", "heat", "cool", "auto"], 7, 30, 0.5),
                  {"current_temp": 24.6, "target_temp": 23.0, "target_temp_low": 20.0, "target_temp_high": 25.0,
                   "action": "cooling", "humidity": 52}),
    "buanderie": dev("Clim buanderie", "proxy", "heat",
                     caps(["off", "heat", "cool", "auto"], 16, 30, 0.5, fan=["auto", "low", "high"]),
                     {"current_temp": 17.5, "target_temp": 20.0, "connected": False}),
}

OTHERS = {
    "temp_salon": {"type": "WITemp", "gui_type": "temp", "name": "Salon", "state": "19.2"},
    "light_salon": {"type": "WODigital", "gui_type": "light", "name": "Plafonnier", "state": "true"},
    "shutter_sejour": {"type": "WOVolet", "gui_type": "shutter", "name": "Volet", "state": "false"},
}
OTYPE = {"temp_salon": "Temperature", "light_salon": "LightSwitch", "shutter_sejour": "Shutter"}


def w(io, x, y, ww=1, hh=1):
    return {"type": OTYPE.get(io, "Climate"), "io_id": io, "x": x, "y": y, "w": ww, "h": hh}


def scenario(widgets, steps, size, grid=None):
    states = {i: copy.deepcopy(D.get(i) or OTHERS[i]) for i in {x["io_id"] for x in widgets}}
    g = grid or ([3, 3] if size == "720x720" else [4, 2])
    return {"grid": g, "pages": [{"widgets": widgets}], "io_states": states, "steps": steps}


def save(name, sc):
    with open(f"{out}/{name}.json", "w") as f:
        json.dump(sc, f, indent=1)


# Small widgets, 720x720, 3x3 grid
save("small", scenario(
    [w("salon", 0, 0, 2, 1), w("chambre", 2, 0, 1, 2), w("bureau", 0, 1), w("cuisine", 1, 1),
     w("sejour", 0, 2), w("buanderie", 1, 2), w("temp_salon", 2, 2)],
    [{"wait": 500}, {"shot": "rui-small-widgets"}], "720x720"))

# Full-screen view of Clim salon (auto), then the two popups
close = [664, 56]
pill = [360, 520]
more = [360, 668]
save("fullscreen", scenario(
    [w("salon", 0, 0)],
    [{"wait": 500}, {"tap": [120, 120]}, {"wait": 500}, {"shot": "rui-fullscreen"},
     {"tap": pill}, {"wait": 400}, {"shot": "rui-mode-popup"},
     {"tap": [360, 615]}, {"wait": 400},
     {"tap": more}, {"wait": 400}, {"shot": "rui-more-popup"}], "720x720"))

# Large widgets
save("large2", scenario(
    [w("sejour", 0, 0, 2, 2), w("bureau", 2, 0), w("cuisine", 2, 1), w("temp_salon", 0, 2),
     w("light_salon", 1, 2), w("shutter_sejour", 2, 2)],
    [{"wait": 500}, {"shot": "rui-large-2x2"}], "720x720"))
save("large3", scenario([w("salon", 0, 0, 3, 3)], [{"wait": 500}, {"shot": "rui-large-3x3"}], "720x720"))

# 800x480, 4x2 grid, climate mixed with other widgets
save("p800", scenario(
    [w("chambre", 0, 0, 1, 2), w("salon", 1, 0, 2, 1), w("bureau", 3, 0), w("light_salon", 1, 1),
     w("sejour", 2, 1), w("shutter_sejour", 3, 1)],
    [{"wait": 500}, {"shot": "rui-800x480"}], "800x480"))
