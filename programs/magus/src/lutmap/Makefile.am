## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_lutmap.la

libmagus_lutmap_la_SOURCES = \
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

libmagus_lutmap_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

libmagus_lutmap_la_LDFLAGS =
