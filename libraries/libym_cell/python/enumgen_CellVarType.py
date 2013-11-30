#! /usr/bin/env python
# coding=UTF-8

import enumgen

# CellDelayModel
map_dict = {
    "type_name"    : "CellVarType",
    "ctype_name"   : "tCellVarType",
    "type_doc"     : "Variable Type",
    "const_prefix" : "kCellVar",
    "default_val"  : "kCellVarNone"
}

type_list = (
  ("InputNetTransition", "input_net_transition"),

  ("TotalOutputNetCapacitance", "total_output_net_capacitance"),
  ("OutputNetLength", "output_net_length"),
  ("OutputNetWireCap", "output_net_wire_cap"),
  ("OutputNetPinCap", "output_net_pin_cap"),

  ("RelatedOutTotalOutputNetCapacitance", "related_out_total_output_net_capacitance"),
  ("RelatedOutOutputNetLength", "related_out_output_net_length"),
  ("RelatedOutOutputNetWireCap", "related_out_output_net_wire_cap"),
  ("RelatedOutOutputNetPinCap", "related_out_output_net_pin_cap"),

  ("ConstrainedPinTransition", "constrained_pin_transition"),

  ("RelatedPinTransition", "related_pin_transition"),

  ("None", "none")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellVarType.cc")
enumgen.make_header(map_dict, type_list, "_CellVarType.h")
