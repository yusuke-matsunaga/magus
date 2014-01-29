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

noinst_LTLIBRARIES = libym_vl_parser_spt.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include


############################################################
## libym_vl_parser_spt.la 用の変数の定義
############################################################

libym_vl_parser_spt_la_SOURCES = \
	SptFactory.h \
	SptUdp.h \
	SptModule.h \
	SptDecl.h \
	SptItem.h \
	SptStmt.h \
	SptExpr.h \
	SptMisc.h \
	SptUdp.cc \
	SptModule.cc \
	SptDecl.cc \
	SptItem.cc \
	SptGenerate.cc \
	SptSpecItem.cc \
	SptStmt.cc \
	SptExpr.cc \
	SptMisc.cc \
	SptFactory.cc

libym_vl_parser_spt_la_LIBADD =

libym_vl_parser_spt_la_LDFLAGS =
