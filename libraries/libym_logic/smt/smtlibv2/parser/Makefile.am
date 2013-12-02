## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## build targets
############################################################
noinst_LTLIBRARIES = \
	libym_smt_parser.la


############################################################
## libym_smt_parser.la
############################################################
libym_smt_parser_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/smt/smtlibv2/include

libym_smt_parser_la_SOURCES = \
	SmtLibScanner.h \
	SmtLibScanner.cc \
	SmtLibNodeImpl.h \
	SmtLibNodeImpl.cc \
	SmtLibParser.cc

libym_smt_parser_la_LIBADD =

libym_smt_parser_la_LDFLAGS =
