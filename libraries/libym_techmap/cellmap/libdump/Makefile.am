## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap_libdump.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_libdump_la_SOURCES = \
	libdump_nsdef.h \
	LdFunc.h \
	LdFuncMgr.h \
	LdFuncMgr.cc \
	LdFuncRep.h \
	LdPatHandler.h \
	LdPatMgr.h \
	LdPatMgr.cc \
	LdPatNode.h \
	LdPatNode.cc \
	LibDump.h \
	LibDump.cc

libym_techmap_libdump_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_npn/libym_npn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_techmap_libdump_la_LDFLAGS =
