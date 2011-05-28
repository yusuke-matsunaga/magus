## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

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
	parsertest


############################################################
## lextest
############################################################
lextest_SOURCES = \
	lextest.cc

lextest_LDADD = \
	../parser/libym_smt_parser.la

lextest_CXXFLAGS = \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser


############################################################
## parsertest
############################################################
parsertest_SOURCES = \
	parsertest.cc

parsertest_LDADD = \
	../parser/libym_smt_parser.la

parsertest_CXXFLAGS = \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser
