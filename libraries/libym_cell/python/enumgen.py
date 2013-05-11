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

# キーワード長の最大値を求める．
max_len = 0
for type in type_list :
    l = len(type)
    if max_len < l :
        max_len = l
#print "max_length = {}".format(max_len)
#print ""

#
# // CellTimingType_kCombinational の実体
# CellTimingTypeObject CellTimingType_kCombinationalStruct = {
#   PyObject_HEAD_INIT(&PyCellTimingType_Type)
#   kCellTimingCombinational
# };

for type in type_list :
    print "CellTimingTypeObject CellTimingType_k{}Struct = {}".format(type, '{')
    print "  PyObject_HEAD_INIT(&PyCellTimingType_Type)"
    print "  kCellTiming{}".format(type)
    print "};"
    print ""
print ""

#
#
# PyObject* CellTimingType_kCombinationalString = NULL;
for type in type_list :
    fill_str = "".ljust(max_len - len(type))
    print "PyObject* CellTimingType_k{}String{} = NULL;".format(type, fill_str)
print ""

#
# case kCellTimingCombinational:    result = CellTimingType_kCombinationalString; break;
for type in type_list :
    fill_str = "".ljust(max_len - len(type))
    print "  case kCellTiming{}:{} result = CellTimingType_k{}String; break;".format(type, fill_str, type)
print ""

#
#  case kCellTimingCombinational:    result = Py_CellTimingType_kCombinational; break;
for type in type_list :
    fill_str = "".ljust(max_len - len(type))
    print "  case kCellTiming{}:{} result = Py_CellTimingType_k{}; break;".format(type, fill_str, type)
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
    fill_str = "".ljust(max_len - len(type))
    print "// kCellTiming{} を表すオブジェクト".format(type)
    print "PyObject* Py_CellTimingType_k{}{} = NULL;".format(type, fill_str)
    print ""
print ""

#
#  timing_type_set(CellTimingType_kCombinationalStruct,    Py_CellTimingType_kCombinational,    m, "kTimingTypeCombinational");
for type in type_list :
    print "  timing_type_set(CellTimingType_k{}Struct,".format(type)
    print "                  Py_CellTimingType_k{}, m,".format(type)
    print "                  \"kTimingType{}\");".format(type)
print ""

#
#  CellTimingType_kCombinationalString    = PyString_FromString("combinational");
for type in type_list :
    type_str = type
    fill_str = "".ljust(max_len - len(type))
    print "  CellTimingType_k{}String{} = PyString_FromString(\"{}\");".format(type, fill_str, type_str)
print ""

#
#/// @brief kCellTimingCombinational を表す PyObject
#extern
#PyObject*
#Py_CellTimingType_kCombinational;
for type in type_list :
    print "/// @brief kCellTiming{}を表す PyObject".format(type)
    print "extern"
    print "PyObject*"
    print "Py_CellTimingType_k{};".format(type)
    print ""
