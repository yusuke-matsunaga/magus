## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 467 2007-03-22 07:54:05Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_zstream.la

libym_zstream_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_zstream_la_SOURCES = \
	zstream.cc

libym_zstream_la_LIBADD =

libym_zstream_la_LDFLAGS = \
	-lz
