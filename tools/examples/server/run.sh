#!/bin/bash
# Loads every example project of static/examples in a real calaos_server, built from a
# calaos_base checkout in its dev container. Nothing is installed on the host.
#
#   run.sh <calaos_base checkout with src/bin/calaos_server built> <its dev image> [work dir]
set -eu
base=$(realpath "$1"); image=$2
here=$(dirname "$(realpath "$0")")
examples=$(realpath "$here/../../../static/examples/climat")
work=${3:-$(mktemp -d "${TMPDIR:-/tmp}/calaos-doc-examples.XXXX")}
mkdir -p "$work"; work=$(realpath "$work")
docker run --rm --network none -u "$(id -u):$(id -g)" \
    -v "$base:/src:ro" -v "$examples:/examples:ro" -v "$work:/work" -v "$here:/tools:ro" "$image" /tools/load.sh
python3 -I "$here/check.py" "$examples" "$work" "$here/expected.txt"
