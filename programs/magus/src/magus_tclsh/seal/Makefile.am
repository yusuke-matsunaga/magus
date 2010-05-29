## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include \
	-I$(top_srcdir)/programs/magus/src/seal \
	-I$(top_srcdir)/programs/magus/src/magus_tclsh/include

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
	$(top_builddir)/libraries/libym_tclpp/libym_tclpp.la

libmagus_seal_tcl_la_LDFLAGS =
