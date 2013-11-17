## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## build targets
############################################################
noinst_LTLIBRARIES = \
	libym_smt_main.la


############################################################
## libym_smt_main.la
############################################################
libym_smt_main_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libym_smt_main_la_SOURCES = \
	print.cc \
	SmtCompTerm.h \
	SmtCompTerm.cc \
	SmtConstTerm.h \
	SmtConstTerm.cc \
	SmtFunTerm.h \
	SmtFunTerm.cc \
	SmtIdImpl.h \
	SmtIdImpl.cc \
	SmtIdMgr.cc \
	SmtMgr.cc \
	SmtNameMgr.h \
	SmtNameMgr.cc \
	SmtListTerm.h \
	SmtListTerm.cc \
	SmtSortImpl.cc \
	SmtSortImpl.h \
	SmtSortMgr.h \
	SmtSortMgr.cc \
	SmtTermImpl.h \
	SmtTermImpl.cc \
	SmtTermMgr.h \
	SmtTermMgr.cc \
	SmtVarFunImpl.h \
	SmtVarFunImpl.cc \
	SmtVarTerm.h \
	SmtVarTerm.cc

libym_smt_main_la_LIBADD =

libym_smt_main_la_LDFLAGS =
