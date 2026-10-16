#!/bin/bash
# Run by run.sh inside the calaos_base container: starts calaos_server for a few seconds on a
# copy of each example, keeps its log and the io.xml it saves on exit.
mkdir -p /work/out
for d in /examples/*/; do
  n=$(basename "$d")
  cfg=$(mktemp -d /tmp/cfg.XXXX); cache=$(mktemp -d /tmp/cache.XXXX)
  cp "$d/io.xml" "$d/rules.xml" "$cfg/"
  timeout -s INT 6 /src/src/bin/calaos_server/calaos_server --config "$cfg" --cache "$cache" > "/work/out/$n.log" 2>&1
  cp "$cfg/io.xml" "/work/out/$n.saved.xml"
done
