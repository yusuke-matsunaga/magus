#! /usr/bin/env python
# coding=UTF-8

import enumgen

# CellDelayModel
map_dict = {
    "type_name"    : "CellDelayModel",
    "ctype_name"   : "tCellDelayModel",
    "type_doc"     : "Delay Model",
    "const_prefix" : "kCellDelay",
    "default_val"  : "kCellDelayGenericCmos"
}

type_list = (
    ("GenericCmos",   "generic_cmos"),
    ("TableLookup",   "table_lookup"),
    ("PiecewiseCmos", "piecewise_cmos"),
    ("Cmos2",         "cmos2"),
    ("Dcm",           "dcm")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellDelayModel.cc")
enumgen.make_header(map_dict, type_list, "_CellDelayModel.h")
