## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_ymsh \
	. \
	tests


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_ymsh.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_ymsh.la 用の変数の定義
############################################################

libym_ymsh_la_SOURCES = \
	Ymsh.cc \
	YmshImpl.h \
	YmshImpl.cc \
	YmshCmdTable.h \
	YmshCmdTable.cc \
	YmshCmd.cc \
	YmshObjTable.h \
	YmshObjTable.cc \
	SetCmd.h \
	SetCmd.cc \
	AliasCmd.h \
	AliasCmd.cc \
	ExitCmd.h \
	ExitCmd.cc \
	DirCmd.h \
	DirCmd.cc

libym_ymsh_la_LIBADD =

libym_ymsh_la_LDFLAGS = \
	-lreadline -lncurses
