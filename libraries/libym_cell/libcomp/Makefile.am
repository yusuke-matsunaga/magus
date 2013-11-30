## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell_libcomp.la

libym_cell_libcomp_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_cell_libcomp_la_SOURCES = \
	libcomp_nsdef.h \
	LcClass.h \
	LcClass.cc \
	LcGroup.h \
	LcGroup.cc \
	LcGroupMgr.h \
	LcGroupMgr.cc \
	LcLogicMgr.h \
	LcLogicMgr.cc \
	LcFFMgr.h \
	LcFFMgr.cc \
	LcLatchMgr.h \
	LcLatchMgr.cc \
	LcPatHandler.h \
	LcPatMgr.h \
	LcPatMgr.cc \
	LcPatNode.h \
	LibComp.h \
	LibComp.cc

libym_cell_libcomp_la_LIBADD =

libym_cell_libcomp_la_LDFLAGS =
