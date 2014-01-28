## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_mvn_tcl.la

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libmagus_mvn_tcl_la_SOURCES = \
	MvnCmd.cc \
	MvnReadVerilog.h \
	MvnReadVerilog.cc \
	mvn_init.cc

libmagus_mvn_tcl_la_LIBADD =

libmagus_mvn_tcl_la_LDFLAGS =
