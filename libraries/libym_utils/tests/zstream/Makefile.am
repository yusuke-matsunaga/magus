## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $


EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	zstream.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

noinst_PROGRAMS = \
	zstream_test \
	GzIDO_test \
	mygzip


zstream_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

zstream_test_SOURCES = \
	zstream_test.cc

zstream_test_LDADD = \
	../../libym_utils.la


GzIDO_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

GzIDO_test_SOURCES = \
	GzIDO_test.cc

GzIDO_test_LDADD = \
	../../libym_utils.la


mygzip_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

mygzip_SOURCES = \
	mygzip.c

mygzip_LDADD = \
	../../libym_utils.la \
	-lbz2 \
	-lz
