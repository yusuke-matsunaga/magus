## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	$(YM_TESTS_SUBDIRS)


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
	lextest \
	parsertest \
	SmtLibShell_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/parser \
	-I$(top_builddir)/parser


############################################################
## lextest 用の変数の指定
############################################################

lextest_SOURCES = \
	lextest.cc

lextest_LDADD = \
	../parser/libym_smt_parser.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

lextest_LDFLAGS =


############################################################
## parsertest 用の変数の指定
############################################################

parsertest_SOURCES = \
	parsertest.cc

parsertest_LDADD = \
	../parser/libym_smt_parser.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

parsertest_LDFLAGS =


############################################################
## SmtLibShell_test 用の変数の指定
############################################################
SmtLibShell_test_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/shell

SmtLibShell_test_SOURCES = \
	SmtLibShell_test.cc

SmtLibShell_test_LDADD = \
	../libym_smtlibv2.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

SmtLibShell_test_LDFLAGS =
