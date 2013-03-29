## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dop.la

libatpg_dop_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(GMP_INCLUDES)

libatpg_dop_la_SOURCES = \
	DopBase.h \
	DopBase.cc \
	DopDrop.h \
	DopDrop.cc \
	DopVerify.h \
	DopVerify.cc

libatpg_dop_la_LDFLAGS =

libatpg_dop_la_LIBADD =
