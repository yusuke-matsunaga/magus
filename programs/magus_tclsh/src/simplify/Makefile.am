## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_simplify.la

libmagus_simplify_la_SOURCES = \
	simplify.h \
	LrwCmd.h \
	LrwCmd.cc
	simplify_init.cc

libmagus_simplify_la_LIBADD =

libmagus_simplify_la_LDFLAGS =
