## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS = \
	ym_mislib \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YFLAGS = -d -v

CLEANFILES = \
	mislib_grammer.cc mislib_grammer.h y.output mislib_grammer.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mislib.la

libym_mislib_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_mislib_la_LDFLAGS =

libym_mislib_la_SOURCES = \
	mislib_grammer.yy \
	MislibLex.h \
	MislibLex.cc \
	MislibMgr.cc \
	MislibMgrImpl.h \
	MislibMgrImpl.cc \
	MislibParser.cc \
	MislibParserImpl.h \
	MislibParserImpl.cc \
	MislibNodeImpl.h \
	MislibNodeImpl.cc \
	MislibNum.h \
	MislibNum.cc \
	MislibStr.h \
	MislibStr.cc \
	MislibSymbol.h \
	MislibSymbol.cc \
	MislibConst.h \
	MislibConst.cc \
	MislibList.h \
	MislibList.cc \
	MislibNot.h \
	MislibNot.cc \
	MislibBop.h \
	MislibBop.cc \
	MislibGate.h \
	MislibGate.cc \
	MislibPin.h \
	MislibPin.cc
