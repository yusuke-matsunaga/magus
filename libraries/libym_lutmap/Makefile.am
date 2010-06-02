## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_lutmap \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_lutmap.la

INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)

libym_lutmap_la_SOURCES = \

	lutmap.h \
	SbjGraph.h \
	SbjGraph.cc \
	SbjGraph_mindepth.cc \
	bnet2sbj.cc \
	LutNetwork.h \
	LutNetwork.cc \
	lut2bnet.cc \
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
	DagACover.h \
	DagACover.cc \
	WeightCover.h \
	WeightCover.cc \
	DagDCover.h \
	DagDCover.cc \
	WeightDCover.h \
	WeightDCover.cc

libym_lutmap_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

libym_lutmap_la_LDFLAGS =
