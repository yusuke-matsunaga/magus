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
	GzIn_test


zstream_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

zstream_test_SOURCES = \
	zstream_test.cc

zstream_test_LDADD = \
	../../libym_utils.la


GzIn_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

GzIn_test_SOURCES = \
	GzIn.h \
	GzIn.cc \
	GzIn_test.cc

GzIn_test_LDADD = \
	../../libym_utils.la
