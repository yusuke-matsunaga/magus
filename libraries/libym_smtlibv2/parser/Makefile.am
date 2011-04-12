## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YFLAGS = -d -v

CLEANFILES = \
	smtlib_grammer.cc smtlib_grammer.h y.output smtlib_grammer.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_smt_parser.la


############################################################
## libym_smt_parser.la
############################################################
libym_smt_parser_la_SOURCES = \
	smtlib_grammer.yy \
	SmtLibLex.cc \
	SmtLibParser.h \
	SmtLibParser.cc

libym_smt_parser_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_smt_parser_la_LDFLAGS =
