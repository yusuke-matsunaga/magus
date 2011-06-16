## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST = $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(TCL_INCLUDES) \
	@POPT_INCLUDES@

YM_TCLPP = $(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

YM_LOGIC = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

#if BUILD_LIBYM_TCLPP
#NPN_CHECK = npn_check
#else
#NPN_CHECK =
#endif

noinst_PROGRAMS = \
	tvfunc_test \
	tvfunc_test2 \
	cofactor_test \
	npn_check3 \
	$(NPN_CHECK)

tvfunc_test_SOURCES = \
	TvFuncTest.h \
	TvFuncTest.cc \
	tvfunc_test.cc

tvfunc_test_LDADD = \
	$(YM_LOGIC) \
	@POPT_LIBS@

tvfunc_test2_SOURCES = \
	tvfunc_test2.cc

tvfunc_test2_LDADD = \
	$(YM_LOGIC) \
	@POPT_LIBS@

cofactor_test_SOURCES = \
	cofactor_test.cc \
	TvFuncTest.h \
	TvFuncTest.cc

cofactor_test_LDADD = \
	$(YM_LOGIC)

npn_check3_SOURCES = \
	npn_check3.cc

npn_check3_LDADD = \
	$(YM_LOGIC) \
	@POPT_LIBS@

#npn_check_SOURCES = \
#	TvFuncTest.h \
#	TvFuncTest.cc \
#	TvFuncConv.h \
#	TvFuncConv.cc \
#	TvFuncTestCmd.h \
#	TvFuncTestCmd.cc \
#	TvFuncTimeCmd.h \
#	TvFuncTimeCmd.cc \
#	FuncCmd.h \
#	FuncCmd.cc \
#	npn_check.cc
#npn_check_LDADD = \
#	$(YM_LOGIC) \
#	$(YM_TCLPP) \
#	$(YM_BASE)
