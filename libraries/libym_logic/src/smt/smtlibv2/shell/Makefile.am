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
	libym_smt_shell.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/smt/smtlibv2/include


############################################################
## libym_smt_shell.la 用の変数の指定
############################################################

libym_smt_shell_la_SOURCES = \
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

libym_smt_shell_la_LIBADD =

libym_smt_shell_la_LDFLAGS =
