## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/seal \
	-I$(top_srcdir)/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_seal_tcl.la

libmagus_seal_tcl_la_SOURCES = \
	SealCmd.h \
	SealCmd.cc \
	TMRCmd.h \
	TMRCmd.cc \
	DupCmd.h \
	DupCmd.cc \
	McaCmd.h \
	McaCmd.cc \
	McaCmd2.h \
	McaCmd2.cc \
	seal_init.cc

libmagus_seal_tcl_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

libmagus_seal_tcl_la_LDFLAGS =
