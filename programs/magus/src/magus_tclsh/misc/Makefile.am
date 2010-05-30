## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 558 2007-04-09 05:54:44Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCL_INCLUDES) \
	$(TK_INCLUDES) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_misc_tcl.la

libmagus_misc_tcl_la_SOURCES = \
	TimeCmd.h \
	TimeCmd.cc \
	RandCmd.h \
	RandCmd.cc \
	misc_init.cc

libmagus_misc_tcl_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la
