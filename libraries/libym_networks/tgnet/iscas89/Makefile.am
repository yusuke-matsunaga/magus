## Process this file with automake to produce Makefile.in

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
