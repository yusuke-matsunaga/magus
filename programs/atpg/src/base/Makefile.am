## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_base.la

libatpg_base_la_SOURCES = \
	SaFault.h \
	SaFault.cc \
	FaultMgr.h \
	FaultMgr.cc \
	TestVector.h \
	TestVector.cc \
	TvMgr.h \
	TvMgr.cc

libatpg_base_la_LDFLAGS =

libatpg_base_la_LIBADD = \
	$(YM_LIBS_LIBYM_TGNET)
