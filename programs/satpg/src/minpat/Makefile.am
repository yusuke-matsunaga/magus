## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_minpat.la

libatpg_minpat_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

libatpg_minpat_la_SOURCES = \
	MinPatImpl.h \
	MinPatImpl.cc \
	GcNode.h \
	GcMgr.h \
	GcMgr.cc

libatpg_minpat_la_LDFLAGS =

libatpg_minpat_la_LIBADD =


noinst_PROGRAMS = \
	gctest

gctest_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

gctest_SOURCES = \
	gctest.cc \
	GcNode.h \
	GcMgr.h \
	GcMgr.cc

gctest_LDADD =

