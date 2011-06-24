## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	libdump \
	.

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap_cell.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_cell_la_SOURCES = \
	cellmap_nsdef.h \
	AreaCover.h \
	AreaCover.cc \
	CellGroup.h \
	CellMap.cc \
	CellMgr.h \
	CellMgr.cc \
	FFClass.h \
	FFClass.cc \
	FFGroup.h \
	FFPosArray.h \
	FFPosArray.cc \
	FuncGroup.h \
	LatchClass.h \
	LatchClass.cc \
	LatchGroup.h \
	LatchPosArray.h \
	LatchPosArray.cc \
	MapRecord.h \
	MapRecord.cc \
	Match.h \
	Match.cc \
	PatGraph.h \
	PatMatcher.h \
	PatMatcher.cc \
	PatMgr.h \
	PatMgr.cc \
	RepFunc.h

libym_techmap_cell_la_LIBADD = \
	libdump/libym_techmap_libdump.la

libym_techmap_cell_la_LDFLAGS =
