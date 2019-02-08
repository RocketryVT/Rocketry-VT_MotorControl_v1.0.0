# transmission.py

import numpy as np
from collections import deque

def parse(bytes):
    parsing = True
    packets = []
    
    while len(bytes) > 0 and parsing:
        while len(bytes) > 0 and bytes[0] != 0xAA:
            bytes.popleft()
        if len(bytes) < 2:
            continue
        if bytes[1] != 0x14:
            bytes.popleft()
            continue
        if len(bytes) < 4:
            parsing = False
            continue
        data_len = bytes[2]
        if len(bytes) < data_len + 6:
            parsing = False
            continue
        packet = []
        for i in range(0, 6 + data_len):
            packet.append(bytes[i])
        packets.append(packet)
        for i in range(0, len(packet)):
            bytes.popleft()
    return packets

def fromFile(filename):
    bytes = open(filename, "rb").read()
    return parse(deque(bytes))

def packet2str(packet):
    str = ""
    if len(packet) < 6:
        return "Packet not long enough"
    isText = packet[3] == 0x23
    for i in range(0, len(packet)):
        if isText and i > 3 and i < len(packet) - 2:
            str = str + chr(packet[i])
        else:
            str = str + "{:02x}".format(packet[i])
        if i < len(packet) - 1 and not(isText and i > 3 and i < len(packet) - 3):
            str = str + " "
        if i == 3 or i == 3 + packet[2]:
            str = str + "| "
    return str




