## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_sat_engine.la

libatpg_sat_engine_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libatpg_sat_engine_la_SOURCES = \
	satpg_engine_nsdef.h \
	InputLiteral.h \
	InputLiteral.cc \
	SatEngineImpl.h \
	SatEngineImpl.cc

libatpg_sat_engine_la_LIBADD =

libatpg_sat_engine_la_LDFLAGS =
