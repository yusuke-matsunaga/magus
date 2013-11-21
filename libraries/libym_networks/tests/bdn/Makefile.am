## Process this file with automake to produce Makefile.in

SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	base_test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_NET = \
	../../libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

bin_PROGRAMS = \
	base_test \
	bliftest \
	change_test

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	$(LIBYM_NET)

bliftest_SOURCES = \
	bliftest.cc

bliftest_LDADD = \
	$(LIBYM_NET)

change_test_SOURCES = \
	change_test.cc

change_test_LDADD = \
	$(LIBYM_NET)
