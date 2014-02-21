## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	bddsh_lex.cc \
	bddsh_parser.hh \
	bddsh_parser.cc \
	y.output \
	bddsh_parser.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libymbddtest.la

noinst_PROGRAMS = \
	mgr_test \
	base_test \
	fdec_test \
	bddsh


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	$(GMP_INCLUDES)

AM_YFLAGS = -d -v


############################################################
## 共通の変数の指定
############################################################

YM_BDD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la


############################################################
## libymbddtest.la 用の変数の指定
############################################################

libymbddtest_la_SOURCES = \
	bddtest.h \
	test_utils.cc

libymbddtest_la_LIBADD =

libymbddtest_la_LDFLAGS =


############################################################
## mgr_test 用の変数の指定
############################################################

mgr_test_SOURCES = \
	mgr_test.cc

mgr_test_LDADD = \
	libymbddtest.la \
	$(YM_BDD)

mgr_test_LDFLAGS =


############################################################
## base_test 用の変数の指定
############################################################

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	libymbddtest.la \
	$(YM_BDD)

base_test_LDFLAGS =


############################################################
## fdec_test 用の変数の指定
############################################################

fdec_test_SOURCES = \
	fdec_test.cc

fdec_test_LDADD = \
	libymbddtest.la \
	$(YM_BDD)

fdec_test_LDFLAGS =


############################################################
## bddsh 用の変数の指定
############################################################

bddsh_SOURCES = \
	bddsh_parser.yy \
	bddsh_lex.ll

bddsh_LDADD = \
	$(YM_BDD)

bddsh_LDFLAGS =
