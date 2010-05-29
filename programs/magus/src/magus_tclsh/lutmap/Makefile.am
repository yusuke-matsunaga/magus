## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include \
	-I$(top_srcdir)/programs/magus/src/lutmap \
	-I$(top_srcdir)/programs/magus/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_lutmap_tcl.la

libmagus_lutmap_tcl_la_SOURCES = \
	LutmapCmd.h \
	LutmapCmd.cc \
	Conv2SbjCmd.cc \
	DumpSbjCmd.cc \
	AreaMapCmd.cc \
	DelayMapCmd.cc \
	DumpLutCmd.cc \
	Conv2BNetCmd.cc \
	WriteVqmCmd.cc

libmagus_lutmap_tcl_la_LIBADD = \
	$(top_builddir)/libraries/libym_tclpp/libym_tclpp.la

libmagus_lutmap_tcl_la_LDFLAGS =
