## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	espresso


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	mincov_test \
	mincov_test_d \
	mincov_test_p


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src


############################################################
## Common Variables
############################################################

LIBS = \
	$(top_builddir)/src/libym_utils.la \
	espresso/libesp.la

LIBS_D = \
	$(top_builddir)/src/libym_utils_d.la \
	espresso/libesp_d.la

LIBS_P = \
	$(top_builddir)/src/libym_utils_p.la \
	espresso/libesp_p.la


############################################################
## Variables for building mincov_test
############################################################

mincov_test_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

mincov_test_SOURCES = \
	mincov_test.cc

mincov_test_LDADD = \
	$(LIBS)

mincov_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building mincov_test
############################################################

mincov_test_d_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

mincov_test_d_SOURCES = \
	mincov_test.cc

mincov_test_d_LDADD = \
	$(LIBS_D)

mincov_test_d_LDFLAGS = \
	$(AM_LDFLAGS) \
	-static


############################################################
## Variables for building mincov_test
############################################################

mincov_test_p_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

mincov_test_p_SOURCES = \
	mincov_test.cc

mincov_test_p_LDADD = \
	$(LIBS_P)

mincov_test_p_LDFLAGS = \
	$(AM_LDFLAGS) \
	-static
