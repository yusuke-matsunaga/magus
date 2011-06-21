## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_tgiscas89reader.la

libym_tgiscas89reader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/iscas89

libym_tgiscas89reader_la_SOURCES = \
	TgIscas89Handler.h \
	TgIscas89Handler.cc \
	TgIscas89Reader.cc

libym_tgiscas89reader_la_LIBADD =

libym_tgiscas89reader_la_LDFLAGS =
