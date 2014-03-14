#!/usr/bin/python -O
import re, sys

def cross(a, b):
    c = (a[1]*b[2] - a[2]*b[1],
         a[2]*b[0] - a[0]*b[2],
         a[0]*b[1] - a[1]*b[0])

    return c

def normalize(vec):
        s = sum([i**2 for i in vec])**.5
        return tuple([v / s for v in vec])

def normal(a, b, c):
        x = tuple([i - j for i, j in zip(b, a)])
        y = tuple([i - j for i, j in zip(c, a)])
        return normalize(cross(x, y))

def main():
	text = open(sys.argv[1]).read()
	v = [(float(x), float(y), float(z)) for x, y, z in re.findall(r'vertex\s+(-?\d+.\d+)\s+(-?\d+.\d+)\s+(-?\d+.\d+)', text)]
	f = [(int(x) - 1, int(y) - 1, int(z) - 1) for x, y, z in re.findall(r'tri\s+(\d+)\s+(\d+)\s+(\d+)', text)]
	vn = [(0, 0, 0)] * len(v)
	for a, b, c in f:
            n = normal(v[a], v[b], v[c])
            vn[a] = tuple([i + j for i, j in zip(n, v[a])])
            vn[b] = tuple([i + j for i, j in zip(n, v[b])])
            vn[c] = tuple([i + j for i, j in zip(n, v[c])])
        print 's 1'
        for vert in v:
            print 'v ' + ' '.join([str(x) for x in vert])
        for vertnorm in vn:
            print 'vn ' + ' '.join([str(x) for x in vertnorm])
        for tri in f:
            print 'f ' + ' '.join([str(x + 1) for x in tri])

if __name__ == '__main__':
    main()
