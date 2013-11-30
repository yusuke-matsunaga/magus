## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

EXTRA_DIST =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	SmtIdMgrTest

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)


noinst_PROGRAMS = \
	run_all_test

noinst_LTLIBRARIES = \
	libym_logic_cpputest.la


############################################################
## run_all_test
############################################################
run_all_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_srcdir)/main \
	$(CPPUTEST_INCLUDES)

run_all_test_SOURCES = \
	main.cc \
	ym_logic_CppUTest.h

run_all_test_LDADD = \
	libym_logic_cpputest.la \
	$(top_builddir)/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

run_all_test_LDFLAGS = \
	$(CPPUTEST_LIBS)


############################################################
## libym_logic_cpputest_la
############################################################
libym_logic_cpputest_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_srcdir)/main \
	$(CPPUTEST_INCLUDES)

libym_logic_cpputest_la_SOURCES = \
	LogExprTest.cc \
	SmtSolverTest.cc
