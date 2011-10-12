## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS = \
	ym_cell \
	ci \
	mislib \
	dotlib \
	libcomp \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_cell.la


############################################################
## Variables for building libym_cell.la
############################################################

libym_cell_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_cell_la_SOURCES = \
	CellDotlibReader.cc \
	CellMislibReader.cc \
	CellFFInfo.cc \
	CellLatchInfo.cc \
	display.cc

libym_cell_la_LIBADD = \
	ci/libym_cell_ci.la \
	dotlib/libym_cell_dotlib.la \
	mislib/libym_cell_mislib.la \
	libcomp/libym_cell_libcomp.la

libym_cell_la_LDFLAGS =
