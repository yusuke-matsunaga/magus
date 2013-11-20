## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_logic_smt.la

libym_logic_smt_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_logic_smt_la_SOURCES = \
	SmtFunImpl.h \
	SmtFunImpl.cc \
	SmtFunValidatorImpl.h \
	SmtFunValidatorImpl.cc \
	SmtSolverImpl.h \
	SmtSolverImpl.cc \
	SmtSortImpl.h \
	SmtSortImpl.cc \
	SmtTermImpl.h \
	SmtTermImpl.cc \
	SmtVarImpl.h \
	SmtVarImpl.cc

libym_logic_smt_la_LIBADD =

libym_logic_smt_la_LDFLAGS =
