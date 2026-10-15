#!/bin/sh
# Builds the capture harness against a calaos_installer tree and writes the installer
# screenshots of the climate pages, plus the demo project they show.
#
#   run.sh <calaos_installer tree> [images dir] [build dir]
#
# Images default to content/climate/images of this doc tree, the build to
# $TMPDIR/calaos-installer-doc-shots. The demo project goes to demo/ next to this script.
set -eu

here=$(cd "$(dirname "$0")" && pwd)
src=$(cd "${1:?usage: run.sh <calaos_installer tree> [images dir] [build dir]}" && pwd)
out=${2:-$here/../../../content/climate/images}
build=${3:-${TMPDIR:-/tmp}/calaos-installer-doc-shots}
qmake=${QMAKE:-qmake6}

mkdir -p "$build" "$out"
cd "$build"
"$qmake" CALAOS_INSTALLER_SRC="$src" "$here/harness.pro" >/dev/null
make -j"${JOBS:-32}" >/dev/null

# Own settings: never read nor write the installer settings of the user running this.
export XDG_CONFIG_HOME="$build/config"
export HOME="$build/home"
mkdir -p "$XDG_CONFIG_HOME" "$HOME"
export QT_QPA_PLATFORM=offscreen
export LANG=fr_FR.UTF-8 LC_ALL=fr_FR.UTF-8

"$build/installer_shots" --out "$(cd "$out" && pwd)" --demo "$here/demo"
echo "source: $(git -C "$src" rev-parse HEAD 2>/dev/null || echo unknown)"
