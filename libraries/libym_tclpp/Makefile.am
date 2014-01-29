## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_tclpp \
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

noinst_LTLIBRARIES = libym_tclpp.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_tclpp.la 用の変数の定義
############################################################

libym_tclpp_la_SOURCES = \
	StrBuf.h \
	TclPoptGroup.h \
	TclObj.cc \
	TclBase.cc \
	TclCmd.cc \
	TclObjMsgHandler.cc \
	TclPopt.cc \
	TclVarTrace.cc \
	TclHandler.cc

libym_tclpp_la_LIBADD =

libym_tclpp_la_LDFLAGS = \
	$(TCL_LDADD)
