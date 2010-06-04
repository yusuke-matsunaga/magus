## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_simplify.la

libmagus_simplify_la_SOURCES = \
	simplify.h \
	LrwCmd.h \
	LrwCmd.cc
	simplify_init.cc

libmagus_simplify_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

libmagus_simplify_la_LDFLAGS =
