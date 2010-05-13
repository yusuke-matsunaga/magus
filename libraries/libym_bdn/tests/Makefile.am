## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(top_srcdir)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	base_test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS = \
	base_test \
	bliftest \
	change_test

base_test_SOURCES = \
	base_test.cc
base_test_LDADD = \
	$(top_builddir)/libym_bdn/libym_bdn.la

bliftest_SOURCES = \
	bliftest.cc

bliftest_LDADD = \
	$(top_builddir)/libym_bdn/libym_bdn.la

change_test_SOURCES = \
	change_test.cc

change_test_LDADD = \
	$(top_builddir)/libym_bdn/libym_bdn.la
