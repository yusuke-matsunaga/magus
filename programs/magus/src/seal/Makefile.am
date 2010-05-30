## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCL_INCLUDES) \
	$(GMP_INCLUDES) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_seal.la

libmagus_seal_la_SOURCES = \
	seal.h \
	BddFsm.h \
	BddFsm.cc \
	IdxMapper.h \
	fsm_analysis.cc \
	LogicSim.h \
	LogicSim.cc \
	fsm_analysis2.cc \
	Matrix.h \
	Matrix.cc \
	gauss_elimination.cc \
	MCAnalysis.h \
	MCAnalysis.cc

libmagus_seal_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

libmagus_seal_la_LDFLAGS = \
	$(GMP_LIBS)
