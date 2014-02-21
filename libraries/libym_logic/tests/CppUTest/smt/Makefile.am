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

noinst_LTLIBRARIES = \
	libym_logic_cpputest_smt.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/smt/smtlibv2/include \
	-I$(top_srcdir)/src/smt/smtlibv2/parser \
	-I$(top_srcdir)/src/smt/smtlibv2/shell \
	$(CPPUTEST_INCLUDES)


############################################################
## libym_logic_cpputest_smt_la 用の変数の指定
############################################################

libym_logic_cpputest_smt_la_SOURCES = \
	IdMgrTest.cc \
	NameMgrTest.cc \
	SmtLibScannerTest.cc \
	SmtSolverTest.cc \
	SortMgrTest.cc

libym_logic_cpputest_smt_la_LIBADD =

libym_logic_cpputest_smt_la_LDFLAGS = \
	$(top_builddir)/src/libym_logic.la
