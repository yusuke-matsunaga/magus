## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =


MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_PROGRAMS = \
	EufMgr_test

noinst_LTLIBRARIES = \
	libeuf.la


############################################################
## Variables for building EufMgr_test
############################################################

EufMgr_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

EufMgr_test_SOURCES = \
	EufMgr_test.cc

EufMgr_test_LDADD = \
	libeuf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## Variables for building libeuf.la
############################################################

libeuf_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libeuf_la_SOURCES = \
	EufBin.h \
	EufBin.cc \
	EufBinMgr.h \
	EufBinMgr.cc \
	EufCon.h \
	EufCon.cc \
	EufDis.h \
	EufDis.cc \
	EufEq.h \
	EufEq.cc \
	EufFunc.h \
	EufFunc.cc \
	EufFuncMgr.h \
	EufFuncMgr.cc \
	EufMgr.cc \
	EufNeg.h \
	EufNeg.cc \
	EufNode.cc \
	EufVar.h \
	EufVar.cc \
	EufVarMgr.h \
	EufVarMgr.cc

libeuf_la_LDFLAGS =

libeuf_la_LIBADD =
