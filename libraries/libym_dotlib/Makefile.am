## Process this file with automake to produce Makefile.in

## $Source:$
## $Author:$
## $Revision:$
## $Date:$

SUBDIRS = \
	ym_dotlib \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_dotlib.la


############################################################
## Variables for building libym_dotlib.la
############################################################

libym_dotlib_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

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
	FuncHandler.h \
	FuncHandler.cc \
	FhScanner.h \
	FhScanner.cc \
	ComplexHandler.h \
	ComplexHandler.cc \
	DefineHandler.h \
	DefineHandler.cc \
	GroupHandler.h \
	GroupHandler.cc \
	HandlerFactory.h \
	HandlerFactory.cc \
	DotlibMgr.cc \
	DotlibMgrImpl.h \
	DotlibMgrImpl.cc \
	DotlibNodeImpl.h \
	DotlibNodeImpl.cc \
	DotlibNode.cc \
	DotlibAttrMap.cc \
	DotlibLibrary.cc \
	DotlibCell.cc \
	DotlibFL.cc \
	DotlibFF.cc \
	DotlibLatch.cc \
	DotlibPin.cc

libym_dotlib_la_LIBADD =
#	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_dotlib_la_LDFLAGS =
