## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 557 2007-04-09 05:05:15Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_test.la

libmagus_test_la_LIBADD = \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBYM_BNET)
libmagus_test_la_LDFLAGS = 
libmagus_test_la_SOURCES = \
	TestCmd.h \
	TestCmd.cc
