## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = \
#	base_test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

noinst_PROGRAMS = \
	pylogic_test

pylogic_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

pylogic_test_SOURCES = \
	pylogic_test.cc

pylogic_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pylogic_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
