## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_techmap_tcl.la

libmagus_techmap_tcl_la_SOURCES = \
	AreaMapCmd.h \
	AreaMapCmd.cc \
	DumpCnCmd.h \
	DumpCnCmd.cc \
	DumpPatCmd.h \
	DumpPatCmd.cc \
	LoadPatCmd.h \
	LoadPatCmd.cc \
	TechmapCmd.h \
	TechmapCmd.cc

libmagus_techmap_tcl_la_LIBADD =

libmagus_techmap_tcl_la_LDFLAGS =
