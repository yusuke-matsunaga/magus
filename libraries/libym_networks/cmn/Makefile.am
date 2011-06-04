## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cmn.la

libym_cmn_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_cmn_la_LDFLAGS =

libym_cmn_la_SOURCES = \
	CmnMgr.cc \
	CmnMgrImpl.h \
	CmnMgrImpl.cc \
	CmnNode.cc \
	CmnNodeDffIn.h \
	CmnNodeDffOut.h \
	CmnNodeInput.h \
	CmnNodeLatchIn.h \
	CmnNodeLatchOut.h \
	CmnNodeLogic.h \
	CmnNodeOutput.h \
	CmnNodePI.h \
	CmnNodePO.h \
	CmnPort.cc

