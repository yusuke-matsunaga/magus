## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

EXTRA_DIST =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS =


noinst_PROGRAMS = \
	run_all_test

############################################################
## run_all_test
############################################################
run_all_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_srcdir)/shell \
	$(CPPUTEST_INCLUDES)

run_all_test_SOURCES = \
	SmtLibScannerTest.cc \
	IdMgrTest.cc \
	SortMgrTest.cc \
	NameMgrTest.cc \
	main.cc

run_all_test_LDADD = \
	$(top_builddir)/libym_smtlibv2.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(CPPUTEST_LIBS)
