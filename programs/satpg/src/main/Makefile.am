## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_main.la

libatpg_main_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(GMP_INCLUDES)

libatpg_main_la_SOURCES = \
	AtpgMgr.cc \
	DtpgSat.cc \
	NormalOp.h \
	NormalOp.cc \
	SkipOp.h \
	SkipOp.cc \
	BtBase.h \
	BtBase.cc \
	BtSimple.h \
	BtSimple.cc \
	BtJustBase.h \
	BtJustBase.cc \
	BtJust1.h \
	BtJust1.cc \
	BtZdd.h \
	BtZdd.cc

libatpg_main_la_LDFLAGS =

libatpg_main_la_LIBADD =
