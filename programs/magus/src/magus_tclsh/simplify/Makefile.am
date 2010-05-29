## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include \
	-I$(top_srcdir)/programs/magus/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_simplify.la

libmagus_simplify_la_SOURCES = \
	simplify.h \
	LrwCmd.h \
	LrwCmd.cc
	simplify_init.cc

libmagus_simplify_la_LIBADD = \
	$(top_builddir)/libraries/libym_bdd/libym_bdd.la \
	$(top_builddir)/libraries/libym_bnet/libym_bnet.la \
	$(top_builddir)/libraries/libym_tclpp/libym_tclpp.la

libmagus_simplify_la_LDFLAGS =
