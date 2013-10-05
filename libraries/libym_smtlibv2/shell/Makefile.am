## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## build targets
############################################################
noinst_LTLIBRARIES = \
	libym_smt_shell.la


############################################################
## libym_smt_shell.la
############################################################
libym_smt_shell_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libym_smt_shell_la_SOURCES = \
	SmtCompTerm.h \
	SmtCompTerm.cc \
	SmtConstTerm.h \
	SmtConstTerm.cc \
	SmtFunImpl.h \
	SmtFunImpl.cc \
	SmtFunMgr.h \
	SmtFunMgr.cc \
	SmtIdImpl.h \
	SmtIdImpl.cc \
	SmtIdMgr.cc \
	SmtIdTerm.h \
	SmtIdTerm.cc \
	SmtLibMgr.h \
	SmtLibMgr.cc \
	SmtLibShell.cc \
	SmtSortImpl.cc \
	SmtSortImpl.h \
	SmtSortMgr.h \
	SmtSortMgr.cc \
	SmtTermImpl.h \
	SmtTermImpl.cc \
	SmtTermList.h \
	SmtTermList.cc

libym_smt_shell_la_LIBADD =

libym_smt_shell_la_LDFLAGS =
