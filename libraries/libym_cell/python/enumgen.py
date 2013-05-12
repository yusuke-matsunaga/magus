#! /usr/bin/env python
# coding=UTF-8

import re
import sys

# キーワード長の最大値を求める．
def calc_max_len(type_list) :
    max_len = 0
    for (type, type_str) in type_list :
        l = len(type)
        if max_len < l :
            max_len = l
    return max_len

# 複数のキーワードの置き換えを行なう．
def multiple_replace(text, adict) :
    rx = re.compile('|'.join(map(re.escape, adict)))
    def one_xlat(match) :
        return adict[match.group(0)]
    return rx.sub(one_xlat, text)

def part_a(map_dict, type_list, output) :
    template1 = """// %%CONST_NAME%% の実体
%%TYPE_NAME%%Object %%CONST_NAME%%Struct = {
  PyObject_HEAD_INIT(&Py%%TYPE_NAME%%_Type)
  %%CONST_NAME%%
};

"""
    max_len = calc_max_len(type_list)
    type_name = map_dict["type_name"]
    const_prefix = map_dict["const_prefix"]
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        adict = {
            "%%CONST_NAME%%"  : const_name,
            "%%TYPE_NAME%%"   : type_name
            }
        output.write( multiple_replace(template1, adict) )
    output.write("// repr 用の文字列オブジェクト\n")
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        fill_str = "".ljust(max_len - len(type))
        output.write("PyObject* {}String{} = NULL;\n".format(const_name, fill_str))
    output.write("\n")
    output.write("// 個々の定数を表す文字列\n")
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        fill_str = "".ljust(max_len - len(type))
        output.write('const char* {}Name{} = "{}";\n'.format(const_name, fill_str, type_str))

def part_b(map_dict, type_list, output) :
    max_len = calc_max_len(type_list)
    const_prefix = map_dict["const_prefix"]
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        fill_str = "".ljust(max_len - len(type))
        output.write("  case {}:{} result = {}String; break;\n".format(const_name, fill_str, const_name))

def part_c(map_dict, type_list, output) :
    max_len = calc_max_len(type_list)
    const_prefix = map_dict["const_prefix"]
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        fill_str = "".ljust(max_len - len(type))
        output.write("  case {}:{} result = Py_{}; break;\n".format(const_name, fill_str, const_name))

def part_d(map_dict, type_list, output) :
    max_len = calc_max_len(type_list)
    const_prefix = map_dict["const_prefix"]
    template1 = """  else if ( strcmp(str, %%CONST_NAME%%Name) == 0 ) {
    result = Py_%%CONST_NAME%%;
  }
"""
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        adict = {"%%CONST_NAME%%"  : const_name }
        output.write( multiple_replace(template1, adict) )

def part_e(map_dict, type_list, output) :
    max_len = calc_max_len(type_list)
    const_prefix = map_dict["const_prefix"]
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        fill_str = "".ljust(max_len - len(type))
        output.write("// {} を表すオブジェクト\n".format(const_name))
        output.write("PyObject* Py_{}{} = NULL;\n".format(const_name, fill_str))
        output.write("\n")

def part_f(map_dict, type_list, output) :
    max_len = calc_max_len(type_list)
    const_prefix = map_dict["const_prefix"]
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        output.write('  obj_set({}Struct,\n'.format(const_name))
        output.write('          Py_{}, m,\n'.format(const_name))
        output.write('          "{}");\n'.format(const_name))
        output.write('\n')

def part_g(map_dict, type_list, output) :
    max_len = calc_max_len(type_list)
    const_prefix = map_dict["const_prefix"]
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        fill_str = "".ljust(max_len - len(type))
        output.write("  {}String{} = PyString_FromString({}Name);\n".format(const_name, fill_str, const_name))

def make_xlat(adict) :
    rx = re.compile('|'.join(map(re.escape, adict)))

    def one_xlat(match) :
        return adict[match.group(0)]

    def xlat(text) :
        return rx.sub(one_xlat, text)

    return xlat


# 列挙型用の Python ラッパを生成する．
def make_cc_source(map_dict, type_list, output_file) :

    adict = {
        "%%TYPE_NAME%%"   : map_dict["type_name"],
        "%%CTYPE_NAME%%"  : map_dict["ctype_name"],
        "%%TYPE_DOC%%"    : map_dict["type_doc"],
        "%%DEFAULT_VAL%%" : map_dict["default_val"]
        }
    translate = make_xlat(adict)

    template_name = "pycell_ENUMTEMPLATE.cc"
    input = open(template_name, 'r')
    output = open(output_file, 'w')
    for line in input :
        if line == '%%%PART_A%%%\n' :
            part_a(map_dict, type_list, output)
        elif line == '%%%PART_B%%%\n' :
            part_b(map_dict, type_list, output)
        elif line == '%%%PART_C%%%\n' :
            part_c(map_dict, type_list, output)
        elif line == '%%%PART_D%%%\n' :
            part_d(map_dict, type_list, output)
        elif line == '%%%PART_E%%%\n' :
            part_e(map_dict, type_list, output)
        elif line == '%%%PART_F%%%\n' :
            part_f(map_dict, type_list, output)
        elif line == '%%%PART_G%%%\n' :
            part_g(map_dict, type_list, output)
        else :
            output.write(translate(line))

def header_part_a(map_dict, type_list, output) :
    const_prefix = map_dict["const_prefix"]
    template1 = """
/// @brief %%CONST_NAME%% を表す Python オブジェクト
extern
PyObject* Py_%%CONST_NAME%%;
"""
    for (type, type_str) in type_list :
        const_name = const_prefix + type
        adict = { "%%CONST_NAME%%" : const_name }
        text = multiple_replace(template1, adict)
        output.write(text)

def make_header(map_dict, type_list, output_file) :

    adict = {
        "%%TYPE_NAME%%"   : map_dict["type_name"],
        "%%CTYPE_NAME%%"  : map_dict["ctype_name"],
        "%%DEFAULT_VAL%%" : map_dict["default_val"]
        }
    translate = make_xlat(adict)

    template_name = "header_ENUMTEMPLATE.h"
    input = open(template_name, 'r')
    output = open(output_file, 'w')
    for line in input :
        if line == '%%%PART_A%%%\n' :
            header_part_a(map_dict, type_list, output)
        else :
            output.write(translate(line))


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

make_cc_source(map_dict, type_list, "_CellDelayModel.cc")
make_header(map_dict, type_list, "_CellDelayModel.h")


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

make_cc_source(map_dict, type_list, "_CellPatType.cc")
make_header(map_dict, type_list, "_CellPatType.h")


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

make_cc_source(map_dict, type_list, "_CellPinDir.cc")
make_header(map_dict, type_list, "_CellPinDir.h")


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

make_cc_source(map_dict, type_list, "_CellTechnology.cc")
make_header(map_dict, type_list, "_CellTechnology.h")
