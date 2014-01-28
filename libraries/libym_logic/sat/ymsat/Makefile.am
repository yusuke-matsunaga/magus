## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


noinst_LTLIBRARIES = libym_sat_ymsat.la


libym_sat_ymsat_la_SOURCES = \
	AssignList.h \
	AssignList.cc \
	SaBase.h \
	SaBase.cc \
	SaFactory.cc \
	SaUIP1.h \
	SaUIP1.cc \
	SaSimple.h \
	SaSimple.cc \
	SatAnalyzer.h \
	SatClause.h \
	SatClause.cc \
	YmSat.h \
	YmSat.cc \
	YmSatR.h \
	YmSatR.cc

libym_sat_ymsat_la_LIBADD =

libym_sat_ymsat_la_LDFLAGS =
