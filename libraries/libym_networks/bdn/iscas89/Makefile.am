## Process this file with automake to produce Makefile.in

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdn_iscas89reader.la

libym_bdn_iscas89reader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/iscas89

libym_bdn_iscas89reader_la_SOURCES = \
	Iscas89BdnConv.h \
	Iscas89BdnConv.cc \
	BdnIscas89Reader.cc

libym_bdn_iscas89reader_la_LIBADD =

libym_bdn_iscas89reader_la_LDFLAGS =
