## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $


SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)


############################################################
## Variables for building targets
############################################################

noinst_PROGRAMS = \
	pyutils_test


############################################################
## Variables for building pyutils_test
############################################################

pyutils_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

pyutils_test_SOURCES = \
	pyutils_test.cc

pyutils_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pyutils_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
