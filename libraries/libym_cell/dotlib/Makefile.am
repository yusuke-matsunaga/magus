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
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_cell_dotlib_la_LDFLAGS =

libym_cell_dotlib_la_SOURCES = \
	dotlib_nsdef.h \
	DotlibParser.h \
	DotlibParser.cc \
	DotlibHandler.h \
	DotlibHandler.cc \
	SimpleHandler.h \
	SimpleHandler.cc \
	ExprHandler.h \
	ExprHandler.cc \
	ComplexHandler.h \
	ComplexHandler.cc \
	DefineHandler.h \
	DefineHandler.cc \
	GroupHandler.h \
	GroupHandler.cc \
	LibraryHandler.h \
	LibraryHandler.cc \
	CellHandler.h \
	CellHandler.cc \
	PtMgr.h \
	PtMgr.cc \
	PtValue.h \
	PtValue.cc \
	PtNode.h \
	PtNodeImpl.h \
	PtNode.cc \
	PtCell.h \
	PtCell.cc \
	PtPin.h \
	PtPin.cc \
	PtDumper.h \
	PtDumper.cc \
	CellDotlibReader.cc



