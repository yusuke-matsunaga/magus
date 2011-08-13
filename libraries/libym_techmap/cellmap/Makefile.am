## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap_cell.la

libym_techmap_cell_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_cell_la_SOURCES = \
	cellmap_nsdef.h \
	AreaCover.h \
	AreaCover.cc \
	CellMap.cc \
	MapRecord.h \
	MapRecord.cc \
	Match.h \
	Match.cc \
	PatMatcher.h \
	PatMatcher.cc

libym_techmap_cell_la_LIBADD =

libym_techmap_cell_la_LDFLAGS =
