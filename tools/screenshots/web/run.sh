#!/usr/bin/env bash
# Usage: run.sh <web-app-tree> [out-dir]
# Rebuilds the app from <web-app-tree> (a calaos-web-app checkout) and takes
# the climate screenshots in French against the demo house.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
tree=$(cd "${1:?usage: run.sh <web-app-tree> [out-dir]}" && pwd)
out=${2:-$here/../../../content/calaos_installer/devices/images}
work=$(mktemp -d "${TMPDIR:-/tmp}/doc5-web-XXXXXX")
trap 'rm -rf "$work"' EXIT

cd "$tree"
[ -d node_modules ] || npm ci
VITE_CONFIG_NATIVE_IGNORE_WARNING=true npx vite build --outDir "$work/dist" --emptyOutDir
node "$here/make-fixtures.mjs" "$work/fixtures"
node "$here/shoot.mjs" "$tree" "$work/fixtures" "$work/dist" "$out"
