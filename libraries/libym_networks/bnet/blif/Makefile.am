## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bnetblifreader.la

libym_bnetblifreader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_bnetblifreader_la_SOURCES = \
	BNetBlifHandler.h \
	BNetBlifHandler.cc \
	BNetBlifReader.cc

libym_bnetblifreader_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la

libym_bnetblifreader_la_LDFLAGS =
