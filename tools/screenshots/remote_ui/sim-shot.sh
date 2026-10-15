#!/bin/bash
# Captures exact screenshots of the calaos_remote_ui SDL simulator, driven
# against a fake Calaos server. Everything runs in a calaos-remote-ui-dev
# container; nothing is installed on the host and --src is never written to.
set -euo pipefail

TOOLS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
IMAGE="${SIM_IMAGE:-calaos-remote-ui-dev:local}"
SIZE=720x720
SRC=
SCENARIO=
OUT=./sim-out
JOBS=32
RUN_TIMEOUT=120
BOOT_WAIT=1500
VERBOSE=0

usage()
{
    cat <<USAGE
Usage: $0 --src DIR --scenario FILE.json [--size 720x720|800x480] [--out DIR]
          [--jobs N] [--timeout SEC] [--boot-wait MS] [--verbose]
Exit status: 0 ok, 1 an expect_* step failed, 2 build/run failure.
USAGE
    exit 64
}

while [ $# -gt 0 ]; do
    case "$1" in
        --src) SRC="$2"; shift 2 ;;
        --scenario) SCENARIO="$2"; shift 2 ;;
        --size) SIZE="$2"; shift 2 ;;
        --out) OUT="$2"; shift 2 ;;
        --jobs) JOBS="$2"; shift 2 ;;
        --timeout) RUN_TIMEOUT="$2"; shift 2 ;;
        --boot-wait) BOOT_WAIT="$2"; shift 2 ;;
        --verbose) VERBOSE=1; shift ;;
        -h|--help) usage ;;
        *) echo "unknown option: $1" >&2; usage ;;
    esac
done

[ -n "$SRC" ] && [ -n "$SCENARIO" ] || usage
case "$SIZE" in 720x720|800x480) ;; *) echo "--size must be 720x720 or 800x480" >&2; exit 64 ;; esac
SRC="$(cd "$SRC" && pwd)"
SCENARIO="$(cd "$(dirname "$SCENARIO")" && pwd)/$(basename "$SCENARIO")"
mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd)"
[ -f "$SRC/boards/linux-dev.cmake" ] || { echo "$SRC is not a calaos_remote_ui tree" >&2; exit 64; }
[ -f "$SCENARIO" ] || { echo "no such scenario: $SCENARIO" >&2; exit 64; }
rm -f "$OUT"/*.png "$OUT"/result.json

HASH="$(printf %s "$SRC" | sha1sum | cut -c1-10)"
KEY="$HASH-$SIZE"
BUILD_ROOT="${SIM_BUILD_ROOT:-$TOOLS/build}"
BUILD="$BUILD_ROOT/$KEY"
BOARDS="$BUILD_ROOT/boards-$KEY"
mkdir -p "$BUILD" "$BOARDS"

# BOARD is spliced into "<src>/boards/<BOARD>.cmake": a relative value reaches the
# generated variant mounted outside the source tree, which therefore stays untouched.
if [ "$SIZE" = 800x480 ]; then
    sed -e 's/^\(set(BOARD_NAME\) .*/\1 "linux-dev-800x480")/' \
        -e 's/^\(set(BOARD_DISPLAY_WIDTH\) .*/\1 800)/' \
        -e 's/^\(set(BOARD_DISPLAY_HEIGHT\) .*/\1 480)/' \
        -e 's/^\(set(BOARD_PREFERED_GRID_WIDTH\) .*/\1 4)/' \
        -e 's/^\(set(BOARD_PREFERED_GRID_HEIGHT\) .*/\1 2)/' \
        "$SRC/boards/linux-dev.cmake" > "$BOARDS/linux-dev-800x480.cmake.new"
    cmp -s "$BOARDS/linux-dev-800x480.cmake.new" "$BOARDS/linux-dev-800x480.cmake" 2>/dev/null \
        && rm "$BOARDS/linux-dev-800x480.cmake.new" \
        || mv "$BOARDS/linux-dev-800x480.cmake.new" "$BOARDS/linux-dev-800x480.cmake"
    BOARD_ARG=../../boards-extra/linux-dev-800x480
else
    : > "$BOARDS/.keep"
    BOARD_ARG=linux-dev
fi

NAME="rui4-sim-$KEY-$$"
cleanup() { docker rm -f "$NAME" >/dev/null 2>&1 || true; }
trap cleanup EXIT INT TERM

# One build dir = one concurrent run.
exec 9>"$BUILD/.lock"
flock 9

T_START=$(date +%s)
set +e
docker run --rm --name "$NAME" -u "$(id -u):$(id -g)" --entrypoint bash \
    -e HOME=/tmp -e BOARD_ARG="$BOARD_ARG" -e JOBS="$JOBS" -e SIZE="$SIZE" \
    -e RUN_TIMEOUT="$RUN_TIMEOUT" -e BOOT_WAIT="$BOOT_WAIT" -e VERBOSE="$VERBOSE" \
    -v "$SRC:/src:ro" -v "$BUILD:/build" -v "$BOARDS:/boards-extra:ro" \
    -v "$TOOLS:/tools:ro" -v "$OUT:/out" -v "$SCENARIO:/scenario.json:ro" \
    "$IMAGE" /tools/in-container.sh
RC=$?
set -e
echo "sim-shot: exit $RC after $(( $(date +%s) - T_START ))s, output in $OUT" >&2
exit "$RC"
