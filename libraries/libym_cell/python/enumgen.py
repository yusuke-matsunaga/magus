#! /usr/bin/env python
# coding=UTF-8

import sys

template_name = sys.argv[1]
type_name = "CellTechnology"
ctype_name = "tCellTechnology"
type_doc = "Technology Type"
const_prefix = "kCellTech"

type_list = (
    ("Cmos", "cmos"),
    ("Fpga", "fpga")
)

# キーワード長の最大値を求める．
max_len = 0
for (type, type_str) in type_list :
    l = len(type)
    if max_len < l :
        max_len = l
#print "max_length = {}".format(max_len)
#print ""

def part_a() :
    for (type, type_str) in type_list :
        print "// {}{} の実体".format(const_prefix, type)
        print "{}Object {}{}Struct = {}".format(type_name, const_prefix, type, '{')
        print "  PyObject_HEAD_INIT(&Py{}_Type)".format(type_name)
        print "  {}{}".format(const_prefix, type)
        print "};"
        print ""
    print ""
    print "// repr 用の文字列オブジェクト"
    for (type, type_str) in type_list :
        fill_str = "".ljust(max_len - len(type))
        print "PyObject* {}{}String{} = NULL;".format(const_prefix, type, fill_str)
    print ""
    print "// 個々の定数を表す文字列"
    for (type, type_str) in type_list :
        fill_str = "".ljust(max_len - len(type))
        print 'const char* {}{}Name = "{}"'.format(const_prefix, type, type_str, fill_str)

def part_b() :
    for (type, type_str) in type_list :
        fill_str = "".ljust(max_len - len(type))
        print "  case {}{}:{} result = {}{}String; break;".format(const_prefix, type, fill_str, const_prefix, type)

def part_c() :
    for (type, type_str) in type_list :
        fill_str = "".ljust(max_len - len(type))
        print "  case {}{}:{} result = Py_{}{}; break;".format(const_prefix, type, fill_str, const_prefix, type)

def part_d() :
    for (type, type_str) in type_list :
        print '  else if ( strcmp(str, {}{}Name) == 0 ) {}'.format(const_prefix, type, '{')
        print '    result = Py_{}{};'.format(const_prefix, type)
        print '  }'

def part_e() :
    for (type, type_str) in type_list :
        fill_str = "".ljust(max_len - len(type))
        print "// {}{} を表すオブジェクト".format(const_prefix, type)
        print "PyObject* Py_{}{}{} = NULL;".format(const_prefix, type, fill_str)
        print ""

def part_f() :
    for (type, type_str) in type_list :
        print "  obj_set({}{}Struct,".format(const_prefix, type)
        print "          Py_{}{}, m,".format(const_prefix, type)
        print "          \"{}Name\");".format(const_prefix, type)

def part_g() :
    for (type, type_str) in type_list :
        fill_str = "".ljust(max_len - len(type))
        print "  {}{}String{} = PyString_FromString(\"{}Name\");".format(const_prefix, type, fill_str, const_prefix, type)


input = open(template_name, 'r')
for line in input :
    line = line.rstrip()
    if line == '%%%PART_A%%%' :
        part_a()
    elif line == '%%%PART_B%%%' :
        part_b()
    elif line == '%%%PART_C%%%' :
        part_c()
    elif line == '%%%PART_D%%%' :
        part_d()
    elif line == '%%%PART_E%%%' :
        part_e()
    elif line == '%%%PART_F%%%' :
        part_f()
    elif line == '%%%PART_G%%%' :
        part_g()
    else :
        print line

#
#/// @brief kCellTimingCombinational を表す PyObject
#extern
#PyObject*
#Py_CellTimingType_kCombinational;
#for type in type_list :
#    print "/// @brief kCellTiming{}を表す PyObject".format(type)
#    print "extern"
#    print "PyObject*"
#    print "Py_CellTimingType_k{};".format(type)
#    print ""
