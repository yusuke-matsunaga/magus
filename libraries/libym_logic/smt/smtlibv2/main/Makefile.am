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
	libym_smt_main.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## libym_smt_main.la 用の変数の指定
############################################################

libym_smt_main_la_SOURCES = \
	print.cc \
	SmtCompTerm.h \
	SmtCompTerm.cc \
	SmtConstTerm.h \
	SmtConstTerm.cc \
	SmtFunTerm.h \
	SmtFunTerm.cc \
	SmtIdImpl.h \
	SmtIdImpl.cc \
	SmtIdMgr.cc \
	SmtMgr.cc \
	SmtNameMgr.h \
	SmtNameMgr.cc \
	SmtListTerm.h \
	SmtListTerm.cc \
	SmtSortImpl.cc \
	SmtSortImpl.h \
	SmtSortMgr.h \
	SmtSortMgr.cc \
	SmtTermImpl.h \
	SmtTermImpl.cc \
	SmtTermMgr.h \
	SmtTermMgr.cc \
	SmtVarFunImpl.h \
	SmtVarFunImpl.cc \
	SmtVarTerm.h \
	SmtVarTerm.cc

libym_smt_main_la_LIBADD =

libym_smt_main_la_LDFLAGS =
