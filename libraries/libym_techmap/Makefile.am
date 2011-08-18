## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_techmap \
	cellmap \
	lutmap \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap.la

libym_techmap_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_la_SOURCES =

libym_techmap_la_LIBADD = \
	cellmap/libym_techmap_cell.la \
	lutmap/libym_techmap_lut.la

libym_techmap_la_LDFLAGS =
