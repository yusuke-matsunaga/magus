## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS = \
	ym_cell \
	mislib \
	dotlib \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell.la

libym_cell_la_LIBADD = \
	dotlib/libym_cell_dotlib.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_cell_la_LDFLAGS = 

libym_cell_la_SOURCES = \
	CellLibrary.cc \
	Cell.cc \
	CellPinImpl.h \
	CellPin.cc \
	CellLutImpl.h \
	CellLut.cc \
	CellTimingImpl.h \
	CellTiming.cc \
	CellManip.h \
	CellManip.cc
