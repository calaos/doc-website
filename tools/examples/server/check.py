#!/usr/bin/env python3
"""usage: check.py <examples dir> <work dir> <expected.txt>. Reads what load.sh left: the server must
start, load every rule, log no warning or error while loading other than the expected ones, and save back every IO
and device of the example."""
import glob, os, re, sys
import xml.etree.ElementTree as ET
NS = '{http://www.calaos.fr}'
examples, work, expected_file = sys.argv[1:4]
# The container has no sidecar, no firmware, no state cache and no MCP wrapper installed.
NOISE = ('Process error: no such file', 'iostates.cache', 'Firmware directory', 'calaos_mcp wrapper')
expected = {}
for line in open(expected_file, encoding='utf-8'):
    if line.strip() and not line.startswith('#'):
        name, pattern = line.rstrip('\n').split('\t', 1)
        expected.setdefault(name, []).append(re.compile(pattern))

def ids(path):
    root = ET.parse(path).getroot()
    return {e.get('id') for e in root.iter() if e.get('id') and e.tag in (NS + 'input', NS + 'output', NS + 'internal', NS + 'device')}

bad = 0
names = sorted(os.path.basename(p.rstrip('/')) for p in glob.glob(os.path.join(examples, '*/')))
for n in names:
    log = open(os.path.join(work, 'out', n + '.log'), errors='replace').read()
    loading = log.split('All services started')[0]
    issues, seen = [], []
    if 'All services started' not in log:
        issues.append('server did not start')
    for l in loading.splitlines():
        if not re.match(r'\[(WRN|ERR|CRI)\]', l) or any(x in l for x in NOISE):
            continue
        hit = [p for p in expected.get(n, []) if p.search(l)]
        (seen if hit else issues).append(l)
    want = len(ET.parse(os.path.join(examples, n, 'rules.xml')).getroot().findall(NS + 'rule'))
    got = re.search(r'Done\. (\d+) rules loaded', log)
    if not got or int(got.group(1)) != want:
        issues.append('%s rules loaded, %d in rules.xml' % (got.group(1) if got else 'no', want))
    missing = ids(os.path.join(examples, n, 'io.xml')) - ids(os.path.join(work, 'out', n + '.saved.xml'))
    if missing:
        issues.append('not saved back: ' + ', '.join(sorted(missing)))
    print(('OK   ' if not issues else 'FAIL ') + n + ' (%d expected warning(s))' % len(seen))
    for l in seen:
        print('     expected: ' + l)
    for i in issues:
        print('     ' + i)
    bad += bool(issues)
print('%d loaded, %d failed' % (len(names), bad))
sys.exit(1 if bad else 0)
