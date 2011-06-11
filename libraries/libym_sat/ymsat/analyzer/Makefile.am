## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1293 2008-02-21 02:25:52Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_sat_analyzer.la

libym_sat_analyzer_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libym_sat_analyzer_la_LIBADD =

libym_sat_analyzer_la_LDFLAGS =

libym_sat_analyzer_la_SOURCES = \
	SaBase.h \
	SaBase.cc \
	SaUIP1.h \
	SaUIP1.cc \
	SaSimple.h \
	SaSimple.cc \
	SaFactory.cc
