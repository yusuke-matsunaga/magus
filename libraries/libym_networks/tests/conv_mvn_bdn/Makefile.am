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

LIBYM_NET = ../../libym_networks.la

bin_PROGRAMS = \
	mvnconv_test

mvnconv_test_SOURCES = \
	mvnconv_test.cc

mvnconv_test_LDADD = \
	$(LIBYM_NET)