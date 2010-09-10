## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_lutmap \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_lutmap.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_lutmap_la_SOURCES = \
	SbjGraph.cc \
	SbjGraph_mindepth.cc \
	LnGraph.cc \
	dump.cc \
	dump_verilog.cc \
	EnumCutOp.h \
	EnumCutOp.cc \
	EnumCut.h \
	EnumCut.cc \
	Cut.h \
	Cut.cc \
	CutHolder.h \
	CutHolder.cc \
	MapRecord.h \
	MapRecord.cc \
	CutResub.h \
	CutResub.cc \
	CrNode.h \
	CrNode.cc \
	CrHeap.h \
	CrHeap.cc \
	CrLevelQ.h \
	CrLevelQ.cc \
	ADCost.h \
	AreaCover.h \
	AreaCover.cc \
	DelayCover.h \
	DelayCover.cc

libym_lutmap_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_lutmap_la_LDFLAGS =
