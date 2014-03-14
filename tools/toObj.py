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
	res = []
	for tri in f:
                ver = (v[tri[0]], v[tri[1]], v[tri[2]])
                res.append((ver, normal(ver[0], ver[1], ver[2])))
        for v, vn in res:
                for i in xrange(3):
                        print 'v ', ' '.join([str(it) for it in v[i]])
                        print 'vn ', ' '.join([str(it) for it in vn])
        for i in xrange(len(res)):
                print 'f ' + ' '.join([str(x) for x in range(3*i + 1, 3*i + 4)])

if __name__ == '__main__':
    main()
