## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_cec.la

libym_cec_la_CPPFLAGS = \
	-I$(top_srcdir)/imp/include \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@GMP_INCLUDES@

libym_cec_la_SOURCES = \
	CecHandle.h \
	CecHash.h \
	CecMgr.h \
	CecMgr.cc \
	CecNode.h
