#!/bin/sh
# Opens every example project of static/examples in the installer of a calaos_installer tree.
#
#   [EXAMPLES_WRITE=1] run.sh <calaos_installer tree> [build dir]
#
# The build defaults to $TMPDIR/calaos-installer-doc-examples (qmake6, make -j32; QMAKE, JOBS
# override). Settings go to the build dir, never to the user's. EXAMPLES_WRITE=1 stores the
# installer's own output in place of a file that differs from it.
set -eu

here=$(cd "$(dirname "$0")" && pwd)
src=$(cd "${1:?usage: run.sh <calaos_installer tree> [build dir]}" && pwd)
build=${2:-${TMPDIR:-/tmp}/calaos-installer-doc-examples}
mkdir -p "$build"
build=$(cd "$build" && pwd)

cd "$build"
"${QMAKE:-qmake6}" CALAOS_INSTALLER_SRC="$src" "$here/check.pro" >/dev/null
make -j"${JOBS:-32}" >/dev/null

export XDG_CONFIG_HOME="$build/config" HOME="$build/home"
mkdir -p "$XDG_CONFIG_HOME" "$HOME"
export QT_QPA_PLATFORM=offscreen

status=0
for d in "$here"/../../../static/examples/climat/*/; do
    "$build/examples_check" "$(cd "$d" && pwd)" 2>/dev/null || status=1
done
echo "source: $(git -C "$src" rev-parse HEAD 2>/dev/null || echo unknown)"
exit $status
