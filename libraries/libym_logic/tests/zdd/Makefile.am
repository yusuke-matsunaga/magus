## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YFLAGS=-d -v

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	base_test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES)

YM_BDD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

noinst_LTLIBRARIES = libymzddtest.la

libymzddtest_la_SOURCES = \
	zddtest.h \
	CombSet.h \
	CombSet.cc \
	test_utils.cc

libymzddtest_la_LIBADD =

libymzddtest_la_LDFLAGS =

noinst_PROGRAMS = \
	base_test


base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	libymzddtest.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la
