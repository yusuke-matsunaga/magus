## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1097 2007-12-21 08:12:52Z matsunaga $

SUBDIRS =

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/iscas89

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
	reader_test

parsertest_SOURCES = \
	parsertest.cc

parsertest_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

reader_test_SOURCES = \
	reader_test.cc

reader_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
