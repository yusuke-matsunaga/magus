## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_gbm \
	pylib \
	. \
	tests

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_gbm.la

# libym_gbm.la
libym_gbm_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_gbm_la_SOURCES = \
	Gbm.cc \
	GbmNodeImpl.h \
	GbmNodeImpl.cc \
	GbmInput.h \
	GbmInput.cc \
	GbmAnd.h \
	GbmAnd.cc \
	GbmLut.h \
	GbmLut.cc

libym_gbm_la_LIBADD =

libym_gbm_la_LDFLAGS =
