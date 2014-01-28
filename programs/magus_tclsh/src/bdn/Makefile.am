## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_logbase_tcl.la

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libmagus_logbase_tcl_la_SOURCES = \
	MvnReadVerilog.h \
	MvnReadVerilog.cc \
	logbase_init.cc

libmagus_logbase_tcl_la_LIBADD =

libmagus_logbase_tcl_la_LDFLAGS =
