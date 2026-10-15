# Installer screenshots (calaos_installer)

`shots.cpp` is compiled with every source of a calaos_installer tree in place of its
`main()` (`harness.pro`), and runs offscreen in French:

1. creates the rooms and plain IOs of `../DEMO_HOUSE.md` in the installer model;
2. creates the five climate devices with the real "New device from a template" dialog
   (`DialogNewDevice`, shipped `data/doc/fr/device_templates.json`), capturing each filled form,
   then a second thermostat on an output already owned (decision 17 refusal);
3. adds the time ranges and the rules of `DEMO_HOUSE.md`, saves `demo/io.xml` and `demo/rules.xml`;
4. reopens that project in the real `MainWindow` and captures the tree, the Add menu, the device
   editor, the rule editor and the action form.

## Run

```sh
./run.sh <calaos_installer tree> [images dir] [build dir]
```

Images default to `content/climate/images/` of this doc tree, the build to
`$TMPDIR/calaos-installer-doc-shots` (qmake6 and `make -j32`; `QMAKE`, `JOBS` override).
Settings go to the build dir, never to the user's. It exits non-zero and lists what went wrong
when a field, template, IO or widget is missing in the tree. `demo/` is rewritten on each run;
the window shows the project as `/tmp/maison-demo` (a copy).

First run against calaos_installer `master` 3950fd2 (INST-4, INST-5, INST-7, INST-8 merged).

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
| inst-rule-circulator.png | Rule "Circulateur : plus aucun thermostat ne chauffe" (3 conditions) |

The device ids are the wizard's (`clim_salon`, `clim_chambre`, `radiateur_bureau`, `pac_cuisine`,
`plancher_sejour`), not the fixture ids of `DEMO_HOUSE.md`. Hosts are 127.0.0.1 (MQTT) and
192.168.1.20 (Wago); no user or password.
