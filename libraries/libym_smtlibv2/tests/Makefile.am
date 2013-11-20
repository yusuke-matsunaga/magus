## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = $(YM_EXTRA_TESTS_SUBDIRS)

CLEANFILES =

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS =


noinst_PROGRAMS = \
	lextest \
	parsertest \
	SmtLibShell_test


############################################################
## lextest
############################################################
lextest_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser

lextest_SOURCES = \
	lextest.cc

lextest_LDADD = \
	../parser/libym_smt_parser.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## parsertest
############################################################
parsertest_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser

parsertest_SOURCES = \
	parsertest.cc

parsertest_LDADD = \
	../parser/libym_smt_parser.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## SmtLibShell_test
############################################################
SmtLibShell_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser \
	-I$(top_srcdir)/shell

SmtLibShell_test_SOURCES = \
	SmtLibShell_test.cc

SmtLibShell_test_LDADD = \
	../libym_smtlibv2.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
