## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_lutmap_tcl.la

libmagus_lutmap_tcl_la_SOURCES = \
	Ln2BNet.h \
	Ln2BNet.cc \
	LutmapCmd.h \
	LutmapCmd.cc \
	AreaMapCmd.cc \
	DelayMapCmd.cc \
	DumpLutCmd.cc \
	Conv2BNetCmd.cc \
	WriteVqmCmd.cc

libmagus_lutmap_tcl_la_LIBADD =

libmagus_lutmap_tcl_la_LDFLAGS =
