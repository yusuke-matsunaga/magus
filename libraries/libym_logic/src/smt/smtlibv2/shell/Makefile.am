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
	libsmtshell.la \
	libsmtshell_d.la \
	libsmtshell_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/smt/smtlibv2/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	IdImpl.h \
	IdImpl.cc \
	IdMgr.h \
	IdMgr.cc \
	NameMgr.h \
	NameMgr.cc \
	NameObj.h \
	NameObjImpl.h \
	NameObjImpl.cc \
	ShellImpl.h \
	ShellImpl.cc \
	SmtLibShell.cc \
	SortElem.h \
	SortElem.cc \
	SortMgr.h \
	SortMgr.cc


############################################################
## libsmtshell.la 用の変数の指定
############################################################

libsmtshell_la_CXXFLAGS = -O2

libsmtshell_la_SOURCES = $(COMMON_SOURCES)

libsmtshell_la_LIBADD =

libsmtshell_la_LDFLAGS = -lreadline -lcurses


############################################################
## libsmtshell_d.la 用の変数の指定
############################################################

libsmtshell_d_la_CXXFLAGS = -O2 -g

libsmtshell_d_la_SOURCES = $(COMMON_SOURCES)

libsmtshell_d_la_LIBADD =

libsmtshell_d_la_LDFLAGS = -lreadline -lcurses


############################################################
## libsmtshell_p.la 用の変数の指定
############################################################

libsmtshell_p_la_CXXFLAGS = -O2 -pg

libsmtshell_p_la_SOURCES = $(COMMON_SOURCES)

libsmtshell_p_la_LIBADD =

libsmtshell_p_la_LDFLAGS = -lreadline -lcurses
