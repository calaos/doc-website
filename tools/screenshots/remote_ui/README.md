# Panel (calaos_remote_ui) climate screenshots

Real captures of the SDL simulator (`linux-dev`), French UI, demo house of `../DEMO_HOUSE.md`.
Everything runs in the `calaos-remote-ui-dev:local` container (source tree mounted read-only,
nothing installed on the host). `sim-shot.sh`, `sim_run.py`, `sim_shim.c` and `in-container.sh`
are the generic capture harness (fake Calaos server, SDL offscreen, input injection);
`gen_scenarios.py` holds the fixtures (spec 5.3 `device` objects, nothing real) and the steps.

Re-run, one command:

    ./run.sh /path/to/calaos_remote_ui_tree [IMAGES_DIR]

`IMAGES_DIR` defaults to `content/climate/images`. Scratch (scenarios, build cache, raw output)
goes under `$SIM_WORK` (default `/tmp/doc5-rui-work`); the build cache is incremental.

Images (720x720, grid 3x3 unless noted): `rui-small-widgets`, `rui-fullscreen`, `rui-mode-popup`,
`rui-more-popup`, `rui-large-2x2`, `rui-large-3x3`, `rui-800x480` (800x480, grid 4x2).

Last run against calaos/calaos_remote_ui `main` at 69bb5d9.

Tap coordinates in `gen_scenarios.py` (full-screen mode pill, "Plus de réglages") depend on the
layout of the full-screen view; adjust them if it changes. Widget names are kept short on purpose:
long names scroll (marquee) and make the capture unstable.
