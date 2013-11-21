#! /usr/bin/env python
# coding=UTF-8

import enumgen

# CellTechnology
map_dict = {
    "type_name"    : "CellTechnology",
    "ctype_name"   : "tCellTechnology",
    "type_doc"     : "Technology Type",
    "const_prefix" : "kCellTech",
    "default_val"  : "kCellTechCmos"
}

type_list = (
    ("Cmos", "cmos"),
    ("Fpga", "fpga")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellTechnology.cc")
enumgen.make_header(map_dict, type_list, "_CellTechnology.h")
