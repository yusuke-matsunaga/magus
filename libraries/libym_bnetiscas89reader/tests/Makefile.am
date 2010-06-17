## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

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

LIBYM_BNET = 

bin_PROGRAMS = \
	iscas89test

iscas89test_SOURCES = \
	iscas89test.cc
iscas89test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnetiscas89reader/libym_bnetiscas89reader.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la
