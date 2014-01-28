## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	expr \
	smt \
	.

CLEANFILES =

EXTRA_DIST =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(CPPUTEST_INCLUDES)


noinst_PROGRAMS = \
	run_all_test

noinst_LTLIBRARIES = \
	libym_logic_cpputest.la


############################################################
## run_all_test
############################################################

run_all_test_SOURCES = \
	main.cc \
	ym_logic_CppUTest.h

run_all_test_LDADD = \
	libym_logic_cpputest.la \
	$(top_builddir)/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

run_all_test_LDFLAGS = \
	$(CPPUTEST_LIBS)


############################################################
## libym_logic_cpputest_la
############################################################

libym_logic_cpputest_la_SOURCES =

libym_logic_cpputest_la_LIBADD = \
	smt/libym_logic_cpputest_smt.la \
	expr/libym_logic_cpputest_expr.la

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_cpputest_la_SOURCES = dummy.cc
