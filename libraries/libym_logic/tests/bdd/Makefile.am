## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

AM_YFLAGS = -d -v

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES = bddsh_lex.cc bddsh_parser.hh bddsh_parser.cc y.output \
		bddsh_parser.output

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	mgr_test \
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

noinst_LTLIBRARIES = libymbddtest.la

libymbddtest_la_SOURCES = \
	bddtest.h \
	test_utils.cc

libymbddtest_la_LIBADD =

libymbddtest_la_LDFLAGS =

noinst_PROGRAMS = \
	mgr_test \
	base_test \
	fdec_test \
	bddsh

mgr_test_SOURCES = \
	mgr_test.cc

mgr_test_LDADD = \
	libymbddtest.la \
	$(YM_BDD)

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	libymbddtest.la \
	$(YM_BDD)

fdec_test_SOURCES = \
	fdec_test.cc

fdec_test_LDADD = \
	libymbddtest.la \
	$(YM_BDD)

bddsh_SOURCES = \
	bddsh_parser.yy \
	bddsh_lex.ll

bddsh_LDADD = \
	$(YM_BDD)
