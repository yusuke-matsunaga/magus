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

noinst_LTLIBRARIES = libym_vl_parser_cpt.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include


############################################################
## libym_vl_parser_cpt.la 用の変数の定義
############################################################

libym_vl_parser_cpt_la_SOURCES = \
	CptUdp.h \
	CptUdp.cc \
	CptModule.h \
	CptModule.cc \
	CptIO.h \
	CptIO.cc \
	CptDecl.h \
	CptDecl.cc \
	CptItem.h \
	CptItem.cc \
	CptInst.h \
	CptInst.cc \
	CptGenerate.h \
	CptGenerate.cc \
	CptSpecItem.h \
	CptSpecItem.cc \
	CptStmt.h \
	CptStmt.cc \
	CptExpr.h \
	CptExpr.cc \
	CptPrimary.cc \
	CptMisc.h \
	CptMisc.cc \
	CptFactory.h \
	CptFactory.cc

libym_vl_parser_cpt_la_LIBADD =

libym_vl_parser_cpt_la_LDFLAGS =
