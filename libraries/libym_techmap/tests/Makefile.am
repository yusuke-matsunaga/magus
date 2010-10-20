## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_techmap

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = \
#	read_blif.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_TECHMAP = $(YMTOOLS_BUILDDIR)/libraries/libym_techmap/libym_techmap.la
LIBYM_BNET = $(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

bin_PROGRAMS = \
	read_blif \
	patgen_test \
	patgraph_test

read_blif_SOURCES = \
	read_blif.cc

read_blif_LDADD = \
	$(LIBYM_TECHMAP) \
	$(LIBYM_BNET)

patgen_test_SOURCES = \
	patgen_test.cc

patgen_test_LDADD = \
	$(LIBYM_TECHMAP)

patgraph_test_SOURCES = \
	patgraph_test.cc

patgraph_test_LDADD = \
	$(LIBYM_TECHMAP)
