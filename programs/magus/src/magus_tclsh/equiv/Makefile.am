## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/equiv \
	-I$(top_srcdir)/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv_tcl.la

libmagus_equiv_tcl_la_SOURCES = \
	EquivCmd.h \
	EquivCmd.cc

libmagus_equiv_tcl_la_LIBADD = \
	$(YM_LIBS_LIBYM_TCLPP)

libmagus_equiv_tcl_la_LDFLAGS =
