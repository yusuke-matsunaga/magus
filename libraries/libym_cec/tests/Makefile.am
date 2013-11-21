## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_BUILDDIR) \
	-I$(YMTOOLS_SRCDIR)/include

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

#LIBYM_NET = $(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la

bin_PROGRAMS = \
	cec_test

cec_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

cec_test_SOURCES = \
	cec_test.cc

cec_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cec/libym_cec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

cec_test_LDFLAGS =
