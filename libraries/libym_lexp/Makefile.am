## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 958 2007-08-28 05:38:02Z matsunaga $

SUBDIRS = \
	. \
	ym_lexp \
	tests

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)

noinst_LTLIBRARIES = libym_lexp.la

libym_lexp_la_LIBADD = \
	$(top_builddir)/libraries/libym_utils/libym_utils.la
libym_lexp_la_LDFLAGS = 
libym_lexp_la_SOURCES = \
	lexp_types.h \
	LexpMgr.h \
	LexpNode.h \
	LexpNodePtr.h \
	LexpParser.h \
	SopLit.h \
	Literal.cc \
	LexpMgr.cc \
	LexpNode.cc \
	LexpParser.cc \
	LogExpr.cc \
	LogExprWriter.cc
