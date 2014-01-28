## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES)

YM_BDD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


noinst_LTLIBRARIES = libymcnfddtest.la

noinst_PROGRAMS = \
	base_test


libymcnfddtest_la_SOURCES = \
	cnfddtest.h \
	test_utils.cc

libymcnfddtest_la_LIBADD =

libymcnfddtest_la_LDFLAGS =


base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	libymcnfddtest.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la
