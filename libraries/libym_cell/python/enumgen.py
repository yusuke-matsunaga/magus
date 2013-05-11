# coding=UTF-8

type_list = (
    "Combinational",
    "CombinationalRise",
    "CombinationalFall",

    "ThreeStateEnable",
    "ThreeStateDisable",
    "ThreeStateEnableRise",
    "ThreeStateEnableFall",
    "ThreeStateDisableRise",
    "ThreeStateDisableFall",

    "RisingEdge",
    "FallingEdge",

    "Preset",
    "Clear",

    "HoldRising",
    "HoldFalling",

    "SetupRising",
    "SetupFalling",

    "RecoveryRising",
    "RecoveryFalling",

    "SkewRising",
    "SkewFalling",

    "RemovalRising",
    "RemovalFalling",

    "NonSeqSetupRising",
    "NonSeqSetupFalling",
    "NonSeqHoldRising",
    "NonSeqHoldFalling",

    "NochangeHighHigh",
    "NochangeHighLow",
    "NochangeLowHigh",
    "NochangeLowLow"
)

#max_len = 0
#for type in type_list :
#    len = type.size()
#    if max_len < len :
#        max_len = len
#print "max_length = {}".format(max_len)
#print ""

#
# // CellTimingType_kCombinational の実体
# CellTimingTypeObject CellTimingType_kCombinationalStruct = {
#   PyObject_HEAD_INIT(&PyCellTimingType_Type)
#   kCellTimingCombinational
# };

for type in type_list :
    print "CellTimingTypeObject CellTmingType_k{}Struct = {}".format(type, '{')
    print "  PyObject_HEAD_INIT(&PyCellTimingType_Type)"
    print "  kCellTiming{}".format(type)
    print "};"
    print ""
print ""

#
#
# PyObject* CellTimingType_kCombinationalString = NULL;
for type in type_list :
    print "PyObject* CellTimingType_k{}String = NULL;".format(type)
print ""

#
# case kCellTimingCombinational:    result = CellTimingType_kCombinationalString; break;
for type in type_list :
    print "  case kCellTiming{}:    result = CellTimingType_k{}String; break;".format(type, type)
print ""

#
#  case kCellTimingCombinational:    result = Py_CellTimingType_kCombinational; break;
for type in type_list :
    print "  case kCellTiming{}:    result = Py_CellTimingType_k{}; break;".format(type, type)
print ""

#
#  else if ( strcmp(str, "combinational") == 0 ) {
#    result = Py_CellTimingType_kCombinational;
#  }
for type in type_list :
    type_str = type
    print "  else if ( strcmp(str, \"{}\") == 0 ) {}".format(type_str, '{')
    print "    result = Py_CellTimingType_k{};".format(type)
    print "  }"
print ""

#
#// kCellTimingCombinational を表すオブジェクト
#PyObject* Py_CellTimingType_kCombinational = NULL;
for type in type_list :
    print "// kCellTiming{} を表すオブジェクト".format(type)
    print "PyObject* Py_CellTimingType_k{} = NULL;".format(type)
    print ""
print ""

#
#  timing_type_set(CellTimingType_kCombinationalStruct,    Py_CellTimingType_kCombinational,    m, "kTimingTypeCombinational");
for type in type_list :
    print "  timing_type_set(CellTimingType_k{}Struct, Py_CellTimingType_k{}, m, \"kTimingType{}\");".format(type, type, type)
print ""

#
#  CellTimingType_kCombinationalString    = PyString_FromString("combinational");
for type in type_list :
    type_str = type
    print "  CellTimingType_k{}String = PyString_FromString(\"{}\");".format(type, type_str)
print ""
