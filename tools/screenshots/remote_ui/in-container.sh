#!/bin/bash
# Runs inside the container: build (incremental), compile the shim, drive the run.
set -euo pipefail

T0=$(date +%s)
say() { echo "[sim-shot $(( $(date +%s) - T0 ))s] $*"; }

# Configuring again would rewrite images_generated.h and recompile its users;
# `cmake --build` re-runs the configure step by itself when a CMakeLists changed.
: >/out/build.log
if [ ! -f /build/CMakeCache.txt ]; then
    say "configure"
    if ! cmake -S /src -B /build -DBUILD_LINUX=ON -DBOARD="$BOARD_ARG" -DBUILD_HOST_TESTS=OFF \
            >>/out/build.log 2>&1; then
        tail -n 40 /out/build.log; exit 2
    fi
fi
say "build"
if ! cmake --build /build -j"$JOBS" >>/out/build.log 2>&1; then
    tail -n 60 /out/build.log; exit 2
fi
[ "$VERBOSE" = 1 ] && tail -n 5 /out/build.log

# Cheap, and keeps the shim in step with the container's SDL headers.
gcc -shared -fPIC -O1 -Wall -o /build/sim_shim.so /tools/sim_shim.c \
    $(sdl2-config --cflags) -lSDL2 -lz -ldl -lpthread

say "run"
python3 -I /tools/sim_run.py --scenario /scenario.json --out /out --size "$SIZE" \
    --binary /build/calaos-remote-ui --shim /build/sim_shim.so \
    --timeout "$RUN_TIMEOUT" --boot-wait "$BOOT_WAIT"
