## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap_sbj.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_sbj_la_SOURCES = \
	SbjDumper.cc \
	SbjGraph.cc \
	SbjMinDepth.h \
	SbjMinDepth.cc \
	SmdNode.h

libym_techmap_sbj_la_LIBADD =

libym_techmap_sbj_la_LDFLAGS =
