## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_smt_parser.la


############################################################
## libym_smt_parser.la
############################################################
libym_smt_parser_la_SOURCES = \
	SmtLibLex.cc \
	SmtLibNodeImpl.h \
	SmtLibNodeImpl.cc \
	SmtLibParser.h \
	SmtLibParser.cc

libym_smt_parser_la_LIBADD =

libym_smt_parser_la_LDFLAGS =
