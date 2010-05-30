## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/equiv \
	-I$(top_srcdir)/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv_tcl.la

libmagus_equiv_tcl_la_SOURCES = \
	EquivCmd.h \
	EquivCmd.cc

libmagus_equiv_tcl_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

libmagus_equiv_tcl_la_LDFLAGS =
