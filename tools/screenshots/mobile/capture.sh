#!/bin/sh
# Climate screenshots of calaos_mobile for the documentation (French, demo house).
#   capture.sh <calaos_mobile source tree> <output dir> [build dir]
# The tree must contain tests/tst_climatewidget (the Qt Quick Test harness); nothing is written into it.
set -eu
SRC=$(realpath "${1:?calaos_mobile source tree}")
OUT=$(realpath -m "${2:?output dir}")
BUILD=$(realpath -m "${3:-${TMPDIR:-/tmp}/doc5-mobile-build}")
HERE=$(cd "$(dirname "$0")" && pwd)

mkdir -p "$OUT" "$BUILD/qml"
for f in DocHouse.qml tst_docshots_phone.qml tst_docshots_desktop.qml; do
    sed "s#@QML@#$SRC/qml#g" "$HERE/$f" > "$BUILD/qml/$f"
done

cd "$BUILD"
qmake6 "$SRC/tests/tst_climatewidget/tst_climatewidget.pro" >/dev/null
make -j"$(nproc)" >/dev/null

# Real render path (RHI/OpenGL) on the offscreen platform.
# (an empty QT_QUICK_BACKEND stops the harness from forcing the software renderer)
export QT_QPA_PLATFORM=offscreen QT_QUICK_BACKEND=${QT_QUICK_BACKEND-} QT_SCALE_FACTOR=1
export CLIMATE_SHOTS_DIR="$OUT"
./tst_climatewidget -input "$BUILD/qml/tst_docshots_phone.qml"
./tst_climatewidget -input "$BUILD/qml/tst_docshots_desktop.qml"

# The dialog is centred on an empty 1280x800 window: keep the dialog and a margin.
if command -v magick >/dev/null 2>&1; then
    magick "$OUT/mobile-desktop-dialog.png" -crop 520x560+380+130 +repage "$OUT/mobile-desktop-dialog.png"
fi
