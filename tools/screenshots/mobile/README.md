# calaos_mobile climate screenshots (French, demo house)

    ./capture.sh <calaos_mobile source tree> <output dir> [build dir]

Example (writes straight into the documentation):

    tools/screenshots/mobile/capture.sh /path/to/calaos_mobile content/climate/images

The script builds the repo's own Qt Quick Test harness (`tests/tst_climatewidget`, qmake6 + make in the build
dir, default `/tmp/doc5-mobile-build`; nothing is written in the source tree) and runs two capture test files
of this directory against it, with the French translation (`lang/calaos_fr.qm`) and the real render path
(RHI/OpenGL on the offscreen platform, 1x). `DocHouse.qml` holds the devices of `../DEMO_HOUSE.md` in the
`device` format of the spec (section 5.3). `@QML@` in the test files is replaced by `<tree>/qml`.

Run against calaos/calaos_mobile master at 4002c3b.

| Image | Content |
|---|---|
| mobile-list.png | phone list: Clim salon (heating, auto), Clim chambre (off), Radiateur bureau (idle), PAC cuisine (lock-out), Plancher sejour (cooling), Clim buanderie (offline) |
| mobile-sheet.png | bottom sheet of Clim salon, auto, two setpoints |
| mobile-sheet-mode.png | mode choice stacked over that sheet |
| mobile-sheet-more.png | "Plus de reglages": fan, swing, humidity |
| mobile-sheet-thermostat.png | bottom sheet of Plancher sejour (thermostat, cool) |
| mobile-desktop-list.png | same list at the width of a desktop room column (366) |
| mobile-desktop-dialog.png | desktop dialog of Clim salon (cropped to the dialog) |
