## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell_dotlib.la

libym_cell_dotlib_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_dotlib/libym_dotlib.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_cell_dotlib_la_LDFLAGS =

libym_cell_dotlib_la_SOURCES = \
	CellDotlibReader.cc
