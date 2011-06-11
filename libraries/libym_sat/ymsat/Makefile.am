## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = \
	analyzer \
	.

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_sat_ymsat.la

libym_sat_ymsat_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libym_sat_ymsat_la_SOURCES = \
	AssignList.h \
	AssignList.cc \
	SatAnalyzer.h \
	SatClause.h \
	SatClause.cc \
	YmSat.h \
	YmSat.cc \
	YmSatR.h \
	YmSatR.cc

libym_sat_ymsat_la_LIBADD = \
	analyzer/libym_sat_analyzer.la

libym_sat_ymsat_la_LDFLAGS =
