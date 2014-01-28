## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


noinst_LTLIBRARIES = libym_sat_dimacs.la


libym_sat_dimacs_la_SOURCES = \
	DimacsParser.cc \
	DimacsParserImpl.h \
	DimacsScanner.h \
	DimacsScanner.cc

libym_sat_dimacs_la_LIBADD =

libym_sat_dimacs_la_LDFLAGS =
