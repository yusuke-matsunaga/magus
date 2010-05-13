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
	parser.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

noinst_PROGRAMS = \
	parsertest \
	nullparser \
	lexptest \
	reader_test

parsertest_SOURCES = \
	parsertest.cc
parsertest_LDADD = \
	$(top_builddir)/libym_blif/libym_blif.la

nullparser_SOURCES = \
	nullparser.cc
nullparser_LDADD = \
	$(top_builddir)/libym_blif/libym_blif.la

lexptest_SOURCES = \
	lexptest.cc
lexptest_LDADD = \
	$(top_builddir)/libym_blif/libym_blif.la \
	$(top_builddir)/libym_lexp/libym_lexp.la

reader_test_SOURCES = \
	reader_test.cc
reader_test_LDADD = \
	$(top_builddir)/libym_blif/libym_blif.la