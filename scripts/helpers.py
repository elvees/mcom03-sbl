# SPDX-License-Identifier: MIT
# Copyright 2023-2024 RnD Center "ELVEES", JSC

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
        raise ValueError(f"{dtb} not in {map_file}")
    return board


def loadbin(file, addr):
    mdb.execute(f"loadbin {file} 0x{addr:x}")


def loadelf(file):
    mdb.execute(f"loadelf {file}")


# Declare setters and getters
def set_32bit(addr, val):
    mdb.execute(f"set 0x{addr:x} 0x{val & 0xFFFFFFFF:x}")


def get_32bit(addr):
    output = mdb.execute(f"show 0x{addr:x}", to_string=True)
    val = int(output.strip().split(":")[1], base=0)
    return val & 0xFFFFFFFF


def set_64bit(addr, val):
    set_32bit(addr, val)
    set_32bit(addr + 4, (val >> 32))


def get_64bit(addr):
    return ((get_32bit(addr + 4) << 32) | get_32bit(addr)) & 0xFFFFFFFFFFFFFFFF
