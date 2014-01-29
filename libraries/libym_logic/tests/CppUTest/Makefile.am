## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	expr \
	smt \
	.


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

noinst_LTLIBRARIES = \
	libym_logic_cpputest.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(CPPUTEST_INCLUDES)


############################################################
## run_all_test 用の変数の指定
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
## libym_logic_cpputest_la 用の変数の指定
############################################################

libym_logic_cpputest_la_SOURCES =

libym_logic_cpputest_la_LIBADD = \
	smt/libym_logic_cpputest_smt.la \
	expr/libym_logic_cpputest_expr.la

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_cpputest_la_SOURCES = dummy.cc
