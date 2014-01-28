## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv_tcl.la

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libmagus_equiv_tcl_la_SOURCES = \
	EquivCmd.h \
	EquivCmd.cc

libmagus_equiv_tcl_la_LIBADD =

libmagus_equiv_tcl_la_LDFLAGS =
