# Example projects (static/examples/climat)

Each folder of `static/examples/climat/` is a complete Calaos project (`io.xml`, `rules.xml`) with its
`LISEZMOI.txt` / `README.txt`; the zip next to it is what the page `calaos_installer/devices/examples`
links. Hardware is replaced by internal variables named "(à remplacer)" so that both programs load
the projects without any device.

| Check | Command |
|---|---|
| calaos_server loads each project: server starts, every rule loaded, every IO saved back, no warning while loading except those of `server/expected.txt` | `server/run.sh <calaos_base checkout built in its dev image> <dev image> [work dir]` |
| Calaos Installer opens each project (given on its command line, as a user would): no message box, no warning, no rule on a missing IO, every device link resolves to an IO its role accepts, the device editor opens, and saving rewrites the files unchanged | `installer/run.sh <calaos_installer tree> [build dir]` (host Qt 6, `qmake6`; `EXAMPLES_WRITE=1` stores the installer's output instead of failing on a difference) |
| the zips match the folders | `zip.py --check` (`zip.py` rewrites them) |

Build calaos_server as in calaos_base's README, in its dev image, e.g.
`docker run --rm -u $(id -u):$(id -g) -v <checkout>:/src -w /src <dev image> bash -c './autogen.sh && ./configure && make -j16'`.

Last run: calaos_base `master` 57474a87, calaos_installer `master` 53d8b84.
