#! /usr/bin/env python
# coding=UTF-8

import enumgen


# CellTimingSense
map_dict = {
    "type_name"    : "CellTimingSense",
    "ctype_name"   : "tCellTimingSense",
    "type_doc"     : "Timing Sense",
    "const_prefix" : "kCell",
    "default_val"  : "kCellNonUnate"
}

type_list = (
    ("PosiUnate", "posi_unate"),
    ("NegaUnate", "nega_unate"),
    ("NonUnate",  "non_unate")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellTimingSense.cc")
enumgen.make_header(map_dict, type_list, "_CellTimingSense.h")
