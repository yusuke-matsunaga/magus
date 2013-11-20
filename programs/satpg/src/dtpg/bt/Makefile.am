## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_bt.la

libatpg_bt_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(GMP_INCLUDES)

libatpg_bt_la_SOURCES = \
	BtBase.h \
	BtBase.cc \
	BtSimple.h \
	BtSimple.cc \
	BtJustBase.h \
	BtJustBase.cc \
	BtJust1.h \
	BtJust1.cc \
	BtJust2.h \
	BtJust2.cc \
	BtZdd.h \
	BtZdd.cc

libatpg_bt_la_LDFLAGS =

libatpg_bt_la_LIBADD =
