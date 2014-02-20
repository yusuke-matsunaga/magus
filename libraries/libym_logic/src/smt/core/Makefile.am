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

noinst_LTLIBRARIES = libym_logic_smt_core.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## libym_logic_smt_core.la 用の変数の指定
############################################################

libym_logic_smt_core_la_SOURCES = \
	SmtFunImpl.h \
	SmtFunImpl.cc \
	SmtFunValidatorImpl.h \
	SmtFunValidatorImpl.cc \
	SmtSolverImpl.h \
	SmtSolverImpl.cc \
	SmtSortImpl.h \
	SmtSortImpl.cc \
	SmtTermImpl.h \
	SmtTermImpl.cc \
	SmtVarImpl.h \
	SmtVarImpl.cc

libym_logic_smt_core_la_LIBADD =

libym_logic_smt_core_la_LDFLAGS =
