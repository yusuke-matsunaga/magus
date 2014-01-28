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
	pycell_test

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

pycell_test_SOURCES = \
	pycell_test.cc

pycell_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/python/libym_cell_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/python/libym_logic_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pycell_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
