## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libphf.la

libphf_la_SOURCES = \
	RvMgr.cc \
	IguGen.h \
	IguGen.cc \
	VectSetList.h \
	Variable.h \
	Variable.cc \
	PhfNode.h \
	PhfEdge.h \
	PhfGen.h \
	PhfGen.cc \
	InputFunc.h \
	VarFunc.h \
	VarFunc.cc \
	XorFunc.h \
	XorFunc.cc

libphf_la_LDFLAGS =

libphf_la_LIBADd =