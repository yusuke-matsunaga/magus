## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_techmap \
	sbjgraph \
	cellmap \
	lutmap \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_la_SOURCES =

libym_techmap_la_LIBADD = \
	sbjgraph/libym_techmap_sbj.la \
	cellmap/libym_techmap_cell.la \
	lutmap/libym_techmap_lut.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdn/libym_bdn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_techmap_la_LDFLAGS =
