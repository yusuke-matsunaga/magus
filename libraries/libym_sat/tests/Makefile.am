## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1293 2008-02-21 02:25:52Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(top_srcdir)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

noinst_PROGRAMS = \
	dimacstest

bin_PROGRAMS = \
	ymsat \
	ymsat2

dimacstest_SOURCES = \
	dimacstest.cc
dimacstest_LDADD = \
	$(top_builddir)/libym_sat/libym_sat.la

ymsat_SOURCES = \
	ymsat.cc
ymsat_LDADD = \
	$(top_builddir)/libym_sat/libym_sat.la

ymsat2_SOURCES = \
	ymsat2.cc
ymsat2_LDADD = \
	$(top_builddir)/libym_sat/libym_sat.la
