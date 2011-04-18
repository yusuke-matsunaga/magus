## Process this file with automake to produce Makefile.in

## $Source:$
## $Author:$
## $Revision:$
## $Date:$

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell_dotlib.la

libym_cell_dotlib_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_cell_dotlib_la_LDFLAGS =

libym_cell_dotlib_la_SOURCES = \
	dotlib_nsdef.h \
	DotLibHandler.h \
	DotLibHandler.cc \
	DotLibLex.cc \
	DotLibLex.h \
	DotLibParser.h \
	DotLibParser.cc \
	LibraryGroupHandler.h \
	LibraryGroupHandler.cc \
	LibSimple.h \
	LibSimple.cc \
	LibComplex.h \
	LibComplex.cc \
	DummySimpleHandler.h \
	DummySimpleHandler.cc \
	DummyComplexHandler.h \
	DummyComplexHandler.cc



