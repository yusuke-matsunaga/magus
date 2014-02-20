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

bin_PROGRAMS =


noinst_PROGRAMS = \
	run_all_test

noinst_LTLIBRARIES = \
	libym_utils_cpputest.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	$(CPPUTEST_INCLUDES)


############################################################
## run_all_test 用の変数の定義
############################################################

run_all_test_SOURCES = \
	main.cc \
	ym_utils_CppUTest.h

run_all_test_LDADD = \
	libym_utils_cpputest.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

run_all_test_LDFLAGS = \
	$(CPPUTEST_LIBS)


############################################################
## libym_utils_cpputest.la
############################################################

libym_utils_cpputest_la_SOURCES = \
	SimpleAllocTest.cc \
	FragAllocTest.cc \
	UnitAllocTest.cc \
	FileTest.cc \
	CombiGenTest.cc \
	PermGenTest.cc \
	HeapTreeTest.cc

libym_utils_cpputest_la_LIBADD =

libym_utils_cpputest_la_LDLFAGS =
