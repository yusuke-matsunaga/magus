## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 557 2007-04-09 05:05:15Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include \
	-I$(top_srcdir)/programs/magus/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_test.la

libmagus_test_la_LIBADD = \
	$(top_builddir)/libraries/libym_tclpp/libym_tclpp.la \
	$(top_builddir)/libraries/libym_bnet/libym_bnet.la
libmagus_test_la_LDFLAGS = 
libmagus_test_la_SOURCES = \
	TestCmd.h \
	TestCmd.cc
