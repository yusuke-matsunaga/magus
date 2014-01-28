## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


noinst_PROGRAMS = \
	pylogic_test

pylogic_test_SOURCES = \
	pylogic_test.cc

pylogic_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/python/libym_logic_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pylogic_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
