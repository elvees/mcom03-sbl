# SPDX-License-Identifier: MIT
# Copyright 2023-2025 RnD Center "ELVEES", JSC

from typing import Optional

import mdb
from elftools.elf.elffile import ELFFile


def get_var_addr(elfpath: str, variable: str) -> Optional[str]:
    """
    The function parses ELF file to find virtual address of variable

    Args:
        elfpath (str):  Path to ELF file
        variable (str): Variable name

    Returns:
        str | None: Variable address
    """
    with open(elfpath, "rb") as f:
        elf = ELFFile(f)

        # Get the symbol table
        symbol_table = elf.get_section_by_name(".symtab")

        # Iterate through symbols to find the variable
        for symbol in symbol_table.iter_symbols():
            if symbol.name == variable and symbol["st_info"]["type"] == "STT_OBJECT":
                return symbol["st_value"]
    return None


def parse_ddrinit_dtb_map(map_file: str, dtb_file: str) -> str:
    """
    The function return board name based on ddrinit and U-Boot DTB

    Args:
        map_file (str): Path to ddrinit MAP file
        dtb_file (str): Path to U-Boot DTB binary

    Raises:
        ValueError: Board is not found

    Returns:
        str: board name
    """
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


def loadbin(file: str, addr: int, size: int = None, offset: int = None):
    """
    loadbin <file> <addr> [(<size> | <offset> <size>)]

    Args:
        file (str):             Path to binary file to be loaded to addr
        addr (int):             Destination addr
        size (int, optional):   Size to be loaded. Defaults to None.
        offset (int, optional): Offset to be loaded from. Defaults to None.
    """
    args = f"{file} 0x{addr & 0xFFFFFFFF:x}"
    if offset is not None and size is not None:
        args += f" 0x{offset & 0xFFFFFFFF:x}"
    if size is not None:
        args += f" 0x{size & 0xFFFFFFFF:x}"
    mdb.execute(f"loadbin {args}")


def loadelf(file: str, sections: str = None, segments: str = None) -> None:
    """
    loadelf <filename> [sections | segments]

    Args:
        file (str): Path to ELF file to be loaded
        sections (str, optional):
            Load by section. Defaults to None.
            Supported since MDB version 8.1.0<7091730> (Jul 15 2025)
        segments (str, optional):
            Load by segments. Defaults to None.
            Supported since MDB version 8.1.0<7091730> (Jul 15 2025)
    """
    args = file
    if sections is None and segments is not None:
        args += f" {sections}"
    elif sections is not None and segments is None:
        args += f" {segments}"
    mdb.execute(f"loadelf {args}")


# Declare setters and getters
def set_by_name(name: str, val: int) -> None:
    """
    set (<regname> | <symbol>) <val>

    Args:
        name (str): Register or symbol name to be set
        val (int):  32 bit value to be set
    """
    mdb.execute(f"set {name} 0x{val & 0xFFFFFFFF:x}")


def get_by_name(name: str) -> int:
    """
    show (<regname> | <symbol>)

    Args:
        name (str): Register or symbol name to be read

    Returns:
        int: 32 bit value
    """
    output = mdb.execute(f"show {name}", to_string=True)
    val = int(output.strip().split()[2], base=0)
    return val & 0xFFFFFFFF


def set_by_addr(addr: int, val: int) -> None:
    """
    set <addr> <val>

    Args:
        addr (int): 32 bit address of 32 bit value to be set
        val (int):  32 bit value to be set
    """
    mdb.execute(f"set 0x{addr & 0xFFFFFFFF:x} 0x{val & 0xFFFFFFFF:x}")


def get_by_addr(addr: int) -> int:
    """
    show <addr>

    Args:
        addr (int): 32 bit address to be read

    Returns:
        int: 32 bit value
    """
    output = mdb.execute(f"show 0x{addr & 0xFFFFFFFF:x}", to_string=True)
    val = int(output.strip().split(":")[1], base=0)
    return val & 0xFFFFFFFF


def set_by_addr64(addr: int, val: int) -> None:
    """
    Set 64 bit value based on set_by_addr.

    Args:
        addr (int): 32 bit address of 64 bit value to be set
        val (int):  64 bit value to be set
    """
    set_by_addr(addr, val)
    set_by_addr(addr + 4, (val >> 32))


def get_by_addr64(addr: int) -> int:
    """
    Get 64 bit value based on get_by_addr.

    Args:
        addr (int): 32 bit address of 64 bit value to be read

    Returns:
        int: 64 bit value
    """
    return (get_by_addr(addr + 4) << 32) | get_by_addr(addr)
