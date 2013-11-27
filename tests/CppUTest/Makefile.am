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


noinst_PROGRAMS = \
	run_all_test


############################################################
## run_all_test
############################################################
run_all_test_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/tests \
	$(CPPUTEST_INCLUDES)

run_all_test_SOURCES = \
	main.cc

run_all_test_LDADD =

if YM_HAVE_LIBYM_LOGIC
run_all_test_LDADD += \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/tests/CppUTest/libym_logic_cpputest.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la
endif

if YM_HAVE_LIBYM_UTILS
run_all_test_LDADD += \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/tests/CppUTest/libym_utils_cpputest.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
endif

run_all_test_LDFLAGS = \
	$(CPPUTEST_LIBS)
