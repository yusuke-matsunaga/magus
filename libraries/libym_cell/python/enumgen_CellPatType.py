#! /usr/bin/env python
# coding=UTF-8

import enumgen

# CellPatType
map_dict = {
    "type_name"    : "CellPatType",
    "ctype_name"   : "tCellPatType",
    "type_doc"     : "Pattern Node Type",
    "const_prefix" : "kCellPat",
    "default_val"  : "kCellPatInput"
}

type_list = (
    ("Input", "input"),
    ("And",   "and"),
    ("Xor",   "xor")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellPatType.cc")
enumgen.make_header(map_dict, type_list, "_CellPatType.h")
