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

noinst_LTLIBRARIES = libymcnfddtest.la

noinst_PROGRAMS = \
	base_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	$(GMP_INCLUDES)


############################################################
## libymcnfbddtest.la 用の変数の指定
############################################################

libymcnfddtest_la_SOURCES = \
	cnfddtest.h \
	test_utils.cc

libymcnfddtest_la_LIBADD =

libymcnfddtest_la_LDFLAGS =


############################################################
## base_test 用の変数の指定
############################################################

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	libymcnfddtest.la \
	$(top_builddir)/src/libym_logic.la

base_test_LDFLAGS =
