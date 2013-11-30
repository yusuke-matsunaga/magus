## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_vl_common.la

libym_vl_common_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/parser/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/elaborator/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/elaborator/ei

libym_vl_common_la_SOURCES = \
	BitVector.cc \
	VlMgr.cc \
	VlValueRep.h \
	VlValue.cc \
	VlUdpVal.cc

libym_vl_common_la_LIBADD =

libym_vl_common_la_LDFLAGS =
