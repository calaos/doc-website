#!/usr/bin/env python3
"""usage: zip.py [--check]. Writes static/examples/climat/<example>.zip from each example folder,
with fixed dates so that an unchanged folder gives the same archive. --check fails when an
archive is not up to date."""
import glob, io, os, sys, zipfile
root = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'static', 'examples', 'climat')
stale = 0
for d in sorted(glob.glob(os.path.join(root, '*/'))):
    name = os.path.basename(d.rstrip('/'))
    buf = io.BytesIO()
    with zipfile.ZipFile(buf, 'w', zipfile.ZIP_DEFLATED) as z:
        for f in sorted(os.listdir(d)):
            info = zipfile.ZipInfo(name + '/' + f, date_time=(2026, 1, 1, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            z.writestr(info, open(os.path.join(d, f), 'rb').read())
    path = os.path.join(root, name + '.zip')
    old = open(path, 'rb').read() if os.path.exists(path) else None
    if old == buf.getvalue():
        continue
    if '--check' in sys.argv:
        print('stale: ' + path)
        stale += 1
    else:
        open(path, 'wb').write(buf.getvalue())
        print('wrote ' + path)
sys.exit(1 if stale else 0)
