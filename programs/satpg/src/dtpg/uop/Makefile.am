## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_uop.la

libatpg_uop_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libatpg_uop_la_SOURCES = \
	UopBase.h \
	UopBase.cc \
	UopSkip.h \
	UopSkip.cc

libatpg_uop_la_LDFLAGS =

libatpg_uop_la_LIBADD =
