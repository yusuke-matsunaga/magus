## Process this file with automake to produce Makefile.in

## $Source:$
## $Author:$
## $Revision:$
## $Date:$

SUBDIRS = \
	ym_dotlib \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_dotlib.la

libym_dotlib_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_dotlib_la_LDFLAGS =

libym_dotlib_la_SOURCES = \
	dotlib_int.h \
	DotlibParser.cc \
	DotlibParserImpl.h \
	DotlibParserImpl.cc \
	DotlibScanner.h \
	DotlibScanner.cc \
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
	HandlerFactory.h \
	HandlerFactory.cc \
	PtMgr.h \
	PtMgr.cc \
	DotlibNodeImpl.h \
	DotlibNodeImpl.cc



