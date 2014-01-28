## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/smt/smtlibv2/include


############################################################
## build targets
############################################################
noinst_LTLIBRARIES = \
	libym_smt_shell.la


############################################################
## libym_smt_shell.la
############################################################

libym_smt_shell_la_SOURCES = \
	IdImpl.h \
	IdImpl.cc \
	IdMgr.h \
	IdMgr.cc \
	NameMgr.h \
	NameMgr.cc \
	NameObj.h \
	NameObjImpl.h \
	NameObjImpl.cc \
	ShellImpl.h \
	ShellImpl.cc \
	SmtLibShell.cc \
	SortElem.h \
	SortElem.cc \
	SortMgr.h \
	SortMgr.cc

libym_smt_shell_la_LIBADD =

libym_smt_shell_la_LDFLAGS =
