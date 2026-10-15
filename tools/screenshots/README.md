# Climate documentation screenshots

Images of `content/calaos_installer/devices/images/` are produced from one invented house (`DEMO_HOUSE.md`), in French,
by the existing capture tools of each client. Nothing here comes from a real installation.

| Client | Directory | Re-run |
|---|---|---|
| calaos-web-app (Playwright) | `web/` | `PLAYWRIGHT_BROWSERS_PATH=<browsers> web/run.sh <web-app checkout> [out-dir]` |
| calaos_mobile (Qt Quick Test, RHI offscreen) | `mobile/` | `mobile/capture.sh <mobile checkout> <out-dir> [build-dir]` |
| calaos_remote_ui (SDL simulator in `calaos-remote-ui-dev:local`) | `remote_ui/` | `remote_ui/run.sh <remote_ui checkout> [out-dir]` |
| calaos_installer (Qt offscreen) | `installer/` | see `installer/README.md` |

`out-dir` defaults to `content/calaos_installer/devices/images`. Each README lists the images its script produces and the
commit it was last run against.
