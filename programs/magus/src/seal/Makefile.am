## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(GMP_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include

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
	$(top_builddir)/libraries/libym_bdd/libym_bdd.la \
	$(top_builddir)/libraries/libym_bnet/libym_bnet.la

libmagus_seal_la_LDFLAGS = \
	$(GMP_LIBS)
