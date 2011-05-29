## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap_lut.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_lut_la_SOURCES = \
	ADCost.h \
	AreaCover.h \
	AreaCover.cc \
	DelayCover.h \
	DelayCover.cc \
	EnumCut.h \
	EnumCut.cc \
	EnumCutOp.h \
	EnumCutOp.cc \
	CrHeap.h \
	CrHeap.cc \
	CrLevelQ.h \
	CrLevelQ.cc \
	CrNode.h \
	CrNode.cc \
	Cut.h \
	Cut.cc \
	CutHolder.h \
	CutHolder.cc \
	CutResub.h \
	CutResub.cc \
	LnGraph.cc \
	LnGraph_dump.cc \
	LutMap.cc \
	MapRecord.h \
	MapRecord.cc

libym_techmap_lut_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_techmap_lut_la_LDFLAGS =
