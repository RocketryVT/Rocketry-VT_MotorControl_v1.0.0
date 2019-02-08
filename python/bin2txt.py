#!/usr/bin/python3

import sys
import transmission

if len(sys.argv) < 2:
    print(sys.argv[0], ": requires filename")
    exit(1)

packets = transmission.fromFile(sys.argv[1]);
for p in packets:
    print(transmission.packet2str(p))
