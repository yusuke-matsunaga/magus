#! /usr/bin/env python
# coding=UTF-8

import enumgen


# CellTimingType
map_dict = {
    "type_name"    : "CellTimingType",
    "ctype_name"   : "tCellTimingType",
    "type_doc"     : "Timing Type",
    "const_prefix" : "kCellTiming",
    "default_val"  : "kCellTimingCombinational"
}

type_list = (
    ("Combinational",         "comibinational"),
    ("CombinationalRise",     "combinational_rise"),
    ("CombinationalFall",     "combinational_fall"),

    ("ThreeStateEnable",      "three_state_enable"),
    ("ThreeStateDisable",     "three_state_disable"),
    ("ThreeStateEnableRise",  "three_state_enable_rise"),
    ("ThreeStateEnableFall",  "three_state_enable_fall"),
    ("ThreeStateDisableRise", "three_state_disable_rise"),
    ("ThreeStateDisableFall", "three_state_disable_fall"),

    ("RisingEdge",            "rising_edge"),
    ("FallingEdge",           "falling_edge"),

    ("Preset",                "preset"),
    ("Clear",                 "clear"),

    ("HoldRising",            "hold_rising"),
    ("HoldFalling",           "hold_falling"),

    ("SetupRising",           "setup_rising"),
    ("SetupFalling",          "setup_falling"),

    ("RecoveryRising",        "recovery_rising"),
    ("RecoveryFalling",       "recovery_falling"),

    ("SkewRising",            "skew_rising"),
    ("SkewFalling",           "skew_falling"),

    ("RemovalRising",         "removal_rising"),
    ("RemovalFalling",        "removal_falling"),

    ("NonSeqSetupRising",     "non_seq_setup_rising"),
    ("NonSeqSetupFalling",    "non_seq_setup_falling"),
    ("NonSeqHoldRising",      "non_seq_hold_rising"),
    ("NonSeqHoldFalling",     "non_seq_hold_falling"),

    ("NochangeHighHigh",      "nochange_high_high"),
    ("NochangeHighLow",       "nochange_high_low"),
    ("NochangeLowHigh",       "nochange_low_high"),
    ("NochangeLowLow",        "nochange_low_low")
)

enumgen.make_cc_source(map_dict, type_list, "pycell_CellTimingType.cc")
enumgen.make_header(map_dict, type_list, "_CellTimingType.h")

