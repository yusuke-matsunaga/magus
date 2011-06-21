## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdn_blifreader.la

libym_bdn_blifreader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/blif

libym_bdn_blifreader_la_LIBADD =

libym_bdn_blifreader_la_LDFLAGS =

libym_bdn_blifreader_la_SOURCES = \
	BdnBlifReader.cc \
	BlifBdnConv.h \
	BlifBdnConv.cc

