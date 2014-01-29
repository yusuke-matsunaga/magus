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
	EufMgr_test

noinst_LTLIBRARIES = \
	libeuf.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## EufMgr_test 用の変数の定義
############################################################

EufMgr_test_SOURCES = \
	EufMgr_test.cc

EufMgr_test_LDADD = \
	libeuf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

EufMgr_test_LDFLAGS =


############################################################
## libeuf.la 用の変数の定義
############################################################

libeuf_la_SOURCES = \
	EufBin.h \
	EufBin.cc \
	EufBinMgr.h \
	EufBinMgr.cc \
	EufBVar.h \
	EufBVar.cc \
	EufCon.h \
	EufCon.cc \
	EufDis.h \
	EufDis.cc \
	EufEq.h \
	EufEq.cc \
	EufFunc.h \
	EufFunc.cc \
	EufFuncMgr.h \
	EufFuncMgr.cc \
	EufMgr.cc \
	EufNeg.h \
	EufNeg.cc \
	EufNode.cc \
	EufVar.h \
	EufVar.cc \
	EufVarMgr.h \
	EufVarMgr.cc

libeuf_la_LIBADD =

libeuf_la_LDFLAGS =
