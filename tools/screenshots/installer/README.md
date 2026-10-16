# Installer screenshots (calaos_installer)

`shots.cpp` is compiled with every source of a calaos_installer tree in place of its
`main()` (`harness.pro`), and runs offscreen in French:

1. creates the rooms and plain IOs of `../DEMO_HOUSE.md` in the installer model;
2. creates the five climate devices with the real "New device from a template" dialog
   (`DialogNewDevice`, shipped `data/doc/fr/device_templates.json`), capturing each filled form,
   then a second thermostat on an output already owned (decision 17 refusal);
3. adds the time ranges and the rules of `DEMO_HOUSE.md`, saves `demo/io.xml` and `demo/rules.xml`;
4. reopens that project in the real `MainWindow` and captures the tree, the Add menu, the device
   editor, the rule editor and the action form;
5. captures the "Find it on the MQTT broker" panel of the ESPHome template against `FakeMqttBroker`
   (the in-process broker of the installer's `tst_esphomediscovery`), with invented retained topics;
6. with `--zones`, builds a second house (`FLOOR_HOUSE` below) saved in `demo/floor/`, and captures
   its wizards, rules, tree and the hot water thermostat.

## Run

```sh
[SHOTS_ARGS=--zones] ./run.sh <calaos_installer tree> [images dir] [build dir]
```

Images default to `content/calaos_installer/devices/images/` of this doc tree, the build to
`$TMPDIR/calaos-installer-doc-shots` (qmake6 and `make -j32`; `QMAKE`, `JOBS` override).
Settings go to the build dir, never to the user's. It exits non-zero and lists what went wrong
when a field, template, IO or widget is missing in the tree. `demo/` is rewritten on each run;
the window shows the project as `/tmp/maison-demo` (a copy).

Qt MQTT is required (the discovery panel is built only with it). On a host without it, run the same
script in the installer CI image (`archlinux` with `qt6-base qt6-5compat qt6-declarative qt6-serialport
qt6-shadertools qt6-tools qt6-mqtt karchive libglvnd gtk3 git`), with the tree writable (the build
writes `lang/*.qm` there) and `-u $(id -u):$(id -g)`.

`--zones` needs a tree whose `data/doc` ships the `thermostat_floor_zone` template (calaos_installer
master since #78).

Last run: calaos_installer `master` 53d8b84.

## Images

| File | What |
|---|---|
| inst-add-menu.png | Ajouter menu on Salon, "Device depuis un template..." highlighted |
| inst-wizard-esphome-climate.png | Clim salon form (two setpoints, fan/swing modes, room sensor) |
| inst-wizard-esphome-climate-ir.png | Clim chambre form |
| inst-wizard-thermostat-heat.png | Radiateur bureau form |
| inst-wizard-thermostat-heat-cool.png | PAC cuisine form |
| inst-wizard-thermostat-floor.png | Plancher séjour form (humidity sensor, 60 %) |
| inst-output-refused.png | Second thermostat on "Relais radiateur bureau": decision 17 message |
| inst-tree-device.png | Whole house tree, devices expanded with their owned IOs |
| inst-device-editor.png | Device editor of Clim salon (links, params, owned IOs), full height |
| inst-rule-complete.png | Rule "Bureau : confort en journée" in the main window |
| inst-action-form.png | Its action opened: generated form (mode + consigne) of Radiateur bureau |
| inst-rule-circulator.png | Rule "Circulateur : toutes les zones éteintes" (5 conditions) |
| inst-discovery-found.png | ESPHome wizard after a search: three climates, one greyed (old node) |
| inst-discovery-filled.png | Same, "clim_salon" chosen: the form filled in |
| inst-wizard-floor-zone.png | Zone de plancher réversible, Séjour (modes off/heat/cool, humidity sensor) |
| inst-wizard-floor-zone-heat.png | Same, Salle de bain (modes off/heat, no humidity sensor) |
| inst-wizard-dhw.png | Thermostat de chauffage "Eau chaude sanitaire" (sensor and relay of the tank) |
| inst-dhw-editor.png | Its device editor, hysteresis to sensor bounds |
| inst-dhw-setpoint.png | Properties of its setpoint IO, `value_max` raised |
| inst-tree-floor.png | Floor house tree: technical room, mode variable, two zones |
| inst-rule-floor-mode.png | "Plancher : mode chaud": variable == 1, nine actions |
| inst-rule-floor-heat-contact.png | "PAC chaud : vanne séjour ouverte" |
| inst-rule-floor-cool-contact.png | "PAC froid : vanne séjour ouverte" |
| inst-rule-floor-all-off.png | "PAC : toutes les vannes fermées" (nine conditions) |
| inst-rule-floor-putback.png | "Plancher séjour : retour en chaud" (condition on the device mode) |

The device ids are the wizard's (`clim_salon`, `clim_chambre`, `radiateur_bureau`, `pac_cuisine`,
`plancher_sejour`), not the fixture ids of `DEMO_HOUSE.md`. Hosts are 127.0.0.1 (MQTT) and
192.168.1.20 (Wago); no user or password.

## FLOOR_HOUSE

Nine zones (Séjour, Cuisine, Bureau, Chambre parents, Chambre 1, Chambre 2 cool and heat; Salle de bain,
Entrée, Buanderie heat only), one Wago relay valve and one MQTT sensor per zone (humidity sensor on
Séjour only), "PAC demande chaud/froid" relays, the InternalInt "Mode plancher" (0 off, 1 heat, 2 cool),
the hot water sensor "Température ballon ECS" and relay "Relais ECS". 34 rules: 3 for the global mode,
9 + 6 + 1 for the heat pump contacts, 6 + 9 for the zones set by hand to the other direction.
