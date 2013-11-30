#! /usr/bin/env python
# coding=UTF-8

import enumgen

# CellPinDir
map_dict = {
    "type_name"    : "CellPinDir",
    "ctype_name"   : "tCellPinDirection",
    "type_doc"     : "Pin Direction",
    "const_prefix" : "kCellPin",
    "default_val"  : "kCellPinInput"
}

type_list = (
    ("Input",    "input"),
    ("Output",   "output"),
    ("Inout",    "inout"),
    ("Internal", "internal")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellPinDir.cc")
enumgen.make_header(map_dict, type_list, "_CellPinDir.h")
