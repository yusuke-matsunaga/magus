## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS =

INCLUDES = \
	-I$(top_srcdir)/include

YFLAGS=-d -v

CLEANFILES = \
	dotlib_grammer.cc dotlib_grammer.h y.output dotlib_grammer.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell_dotlib.la

libym_cell_dotlib_la_LIBADD = \
	$(top_builddir)/libraries/libym_lexp/libym_lexp.la \
	$(top_builddir)/libraries/libym_utils/libym_utils.la

libym_cell_dotlib_la_LDFLAGS = 

libym_cell_dotlib_la_SOURCES = \
	DotLibLex.cc \
	DotLibLex.h
