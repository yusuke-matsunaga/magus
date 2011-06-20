## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS =

YFLAGS = -d -v

CLEANFILES = \
	mislib_grammer.cc mislib_grammer.h y.output mislib_grammer.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_cell_mislib.la


############################################################
## Variables for building libym_cell_mislib.la
############################################################

libym_cell_mislib_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_cell_mislib_la_SOURCES = \
	mislib_nsdef.h \
	mislib_grammer.yy \
	MislibLex.h \
	MislibLex.cc \
	MislibMgr.h \
	MislibMgr.cc \
	MislibMgrImpl.h \
	MislibMgrImpl.cc \
	MislibParser.h \
	MislibParser.cc \
	MislibParserImpl.h \
	MislibParserImpl.cc \
	MislibNode.h \
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

libym_cell_mislib_la_LIBADD =

libym_cell_mislib_la_LDFLAGS =

