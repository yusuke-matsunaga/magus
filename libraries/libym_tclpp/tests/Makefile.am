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

noinst_PROGRAMS = \
	ym_tclsh


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	$(TCLRL_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## ym_tclsh 用の変数の定義
############################################################

ym_tclsh_SOURCES = \
	tclAppInit.cc \
	HelloCmd.h \
	HelloCmd.cc \
	RandCmd.h \
	RandCmd.cc \
	TimeCmd.h \
	TimeCmd.cc \
	OptTestCmd.h \
	OptTestCmd.cc

ym_tclsh_LDADD = \
	$(top_builddir)/src/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

ym_tclsh_LDFLAGS =
