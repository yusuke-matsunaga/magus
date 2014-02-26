## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


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
	run_all_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libym_utils/include \
	-I$(top_builddir)/libraries/libym_utils/include \
	$(CPPUTEST_INCLUDES)


############################################################
## run_all_test 用の変数の定義
############################################################

run_all_test_SOURCES = \
	main.cc

run_all_test_LDADD =

if YM_HAVE_LIBYM_LOGIC

AM_CPPFLAGS += \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/tests

run_all_test_LDADD += \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/tests/CppUTest/libym_logic_cpputest.la
endif

if YM_HAVE_LIBYM_UTILS

AM_CPPFLAGS += \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/tests

run_all_test_LDADD += \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/tests/CppUTest/libym_utils_cpputest.la
endif

run_all_test_LDFLAGS = \
	$(CPPUTEST_LIBS)
