# SPDX-License-Identifier: MIT
# Copyright 2023 RnD Center "ELVEES", JSC

import mdb


def parse_ddrinit_dtb_map(map_file, dtb_file):
    with open(map_file) as f:
        map_ = f.read()
    dtb = dtb_file.strip(".dtb")

    board = None
    for line in map_.splitlines():
        if dtb not in line:
            continue
        board = line.split(":")[0]
        break

    if not board:
        raise ValueError("{0} not in {1}".format(dtb, map_file))
    return board


def loadbin(file, addr):
    mdb.execute("loadbin {0} 0x{1:x}".format(file, addr))


# Declare setters and getters
def set_32bit(addr, val):
    mdb.execute("set 0x{0:x} 0x{1:x}".format(addr, val & 0xFFFFFFFF))


def get_32bit(addr):
    output = mdb.execute("show 0x{0:x}".format(addr), to_string=True)
    val = int(output.strip().split(":")[1], base=0)
    return val & 0xFFFFFFFF


def set_64bit(addr, val):
    set_32bit(addr, val)
    set_32bit(addr + 4, (val >> 32))


def get_64bit(addr):
    return ((get_32bit(addr + 4) << 32) | get_32bit(addr)) & 0xFFFFFFFFFFFFFFFF
