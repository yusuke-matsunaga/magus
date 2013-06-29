## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(GMP_INCLUDES)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libphf.la

libphf_la_SOURCES = \
	RvMgr.cc \
	IguGen.cc \
	IguGen2.cc \
	VectSetList.h \
	Variable.cc \
	PhfGraph.cc \
	PhfNode.h \
	PhfEdge.h \
	PhfGen.cc \
	IGU.cc \
	VarFunc.cc \
	XorFunc.cc

libphf_la_LDFLAGS =

libphf_la_LIBADd =