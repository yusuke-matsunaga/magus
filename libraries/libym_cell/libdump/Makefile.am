## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell_libdump.la

libym_cell_libdump_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_cell_libdump_la_SOURCES = \
	libdump_nsdef.h \
	LdFFClass.h \
	LdFFGroup.h \
	LdFFMgr.h \
	LdFFMgr.cc \
	LdFuncClass.h \
	LdFuncGroup.h \
	LdFuncMgr.h \
	LdFuncMgr.cc \
	LdPatHandler.h \
	LdPatMgr.h \
	LdPatMgr.cc \
	LdPatNode.h \
	LdPatNode.cc \
	LibDump.h \
	LibDump.cc

libym_cell_libdump_la_LIBADD =

libym_cell_libdump_la_LDFLAGS =
