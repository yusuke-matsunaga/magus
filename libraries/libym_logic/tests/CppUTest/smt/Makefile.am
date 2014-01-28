## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

EXTRA_DIST =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/smt/smtlibv2/include \
	-I$(top_srcdir)/smt/smtlibv2/parser \
	-I$(top_srcdir)/smt/smtlibv2/shell \
	$(CPPUTEST_INCLUDES)


noinst_LTLIBRARIES = \
	libym_logic_cpputest_smt.la


############################################################
## libym_logic_cpputest_smt_la
############################################################

libym_logic_cpputest_smt_la_SOURCES = \
	IdMgrTest.cc \
	NameMgrTest.cc \
	SmtLibScannerTest.cc \
	SmtSolverTest.cc \
	SortMgrTest.cc
