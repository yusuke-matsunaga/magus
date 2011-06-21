## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bnetiscas89reader.la

libym_bnetiscas89reader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/iscas89

libym_bnetiscas89reader_la_SOURCES = \
	BNetIscas89Handler.h \
	BNetIscas89Handler.cc \
	BNetIscas89Reader.cc

libym_bnetiscas89reader_la_LIBADD =

libym_bnetiscas89reader_la_LDFLAGS =
