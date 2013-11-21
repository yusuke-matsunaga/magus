## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 697 2007-05-28 17:10:53Z matsunaga $

AM_YFLAGS = -d -v

CLEANFILES = \
	bb_grammer.cc bb_grammer.hh y.output bb_grammer.output \
	bblex.cc

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libbb_parser.la

libbb_parser_la_CPPFLAGS = \
	-I $(YMTOOLS_SRCDIR)/include \
	-I $(YMTOOLS_BUILDDIR)/include \
	-I $(top_srcdir)/src/bb/include

libbb_parser_la_SOURCES = \
	bb_grammer.yy \
	Lexer.h \
	Lexer.cc \
	Driver.cc

libbb_parser_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
