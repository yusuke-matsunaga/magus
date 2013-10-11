## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

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
	SmtShell_test


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
## SmtShell_test
############################################################
SmtShell_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser \
	-I$(top_srcdir)/shell

SmtShell_test_SOURCES = \
	SmtShell_test.cc

SmtShell_test_LDADD = \
	../libym_smtlibv2.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## SmtLibShell
############################################################
SmtLibShell_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser \
	-I$(top_srcdir)/shell

SmtLibShell_SOURCES = \
	SmtLibShell.cc

SmtLibShell_LDADD = \
	../shell/libym_smt_shell.la \
	../parser/libym_smt_parser.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lreadline -lncurses
