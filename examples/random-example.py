#!/usr/bin/python

import sys
import random

if len(sys.argv) < 4:
    print "usage: %s <vertices> <edges> <source>" % sys.argv[0]
    sys.exit(1)

vertices = int(sys.argv[1])
edges = int(sys.argv[2])
source = int(sys.argv[3])

print vertices
print edges
print source

if edges == vertices ** 2:
    # generate a complete graph
    for source in range(1, vertices + 1):
        for sink in range(1, vertices + 1):
            print "%d %d %d" % (source, sink, random.randint(0, 1024))
elif edges == vertices ** 2 - vertices:
    # complete, but no loops
    for source in range(1, vertices + 1):
        for sink in range(1, vertices + 1):
            if source != sink:
                print "%d %d %d" % (source, sink, random.randint(0, 1024))
else:
    # use random edges
    done = [(i, i) for i in range(1, vertices + 1)]
    while edges > 0:
        source = random.randint(1, vertices)
        sink = random.randint(1, vertices)
        if (source, sink) not in done:
            print "%d %d %d" % (source, sink, random.randint(0, 1024))
            done += [(source, sink)]
            edges -= 1
