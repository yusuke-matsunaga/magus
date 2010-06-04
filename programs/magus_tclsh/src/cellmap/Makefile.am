## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_cellmap.la

libmagus_cellmap_la_SOURCES = \
	cellmap.h \
	SbjGraph.h \
	SbjGraph.cc \
	SbjGraph_mindepth.cc \
	CellNetwork.h \
	CellNetwork.cc \
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
	ADCost.h \
	DagCover.h \
	DagACover.h \
	DagACover.cc \
	WeightCover.h \
	WeightCover.cc \
	DagDCover.h \
	DagDCover.cc \
	WeightDCover.h \
	WeightDCover.cc \
	CellmapCmd.h \
	CellmapCmd.cc \
	Conv2SbjCmd.cc \
	DumpSbjCmd.cc \
	AreaMapCmd.cc \
	DelayMapCmd.cc \
	Conv2BNetCmd.cc

libmagus_cellmap_la_LIBADD = \
	$(YM_LIBS_LIBYM_BDD) \
	$(YM_LIBS_LIBYM_BNET) \
	$(YM_LIBS_LIBYM_TCLPP)

libmagus_cellmap_la_LDFLAGS =
