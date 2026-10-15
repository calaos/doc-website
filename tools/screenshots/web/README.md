# calaos-web-app climate screenshots (French, 1280x800)

    PLAYWRIGHT_BROWSERS_PATH=<dir with chromium> ./run.sh <calaos-web-app checkout> [out-dir]

`out-dir` defaults to `content/calaos_installer/devices/images`. The script builds the app into a temp dir (the tree's committed
`dist/` is untouched), generates the demo-house fixtures (`make-fixtures.mjs`, following DEMO_HOUSE.md and the
wire format of spec 5.3), starts a copy of the tree's mock server and `vite preview` on ports 5601/4601
(`MOCK_PORT`, `APP_PORT`), and drives Chromium (`shoot.mjs`). Nothing in the app tree is modified.

Last run: calaos-web-app `master` at 32c28bb (WEB-3 + WEB-4 merged).

Images: web-room-list, web-settings-panel, web-settings-single, web-mode-choice, web-more-settings,
web-thermostat, web-range-stop, web-fault.

Notes
- web-room-list uses a showcase layout: every status in the Salon room (the real house spreads these devices over
  several rooms, the other images use the real layout). The room-list viewport is 1280x900 to fit all rows.
- web-range-stop is reached through the UI: Clim salon, low end raised 19.5 to 22.5 against high 24.5 (min_range 2).
- web-fault uses "Sonde muette" (`sensor_stale`) on Radiateur bureau.
