## Process this file with automake to produce Makefile.in

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
