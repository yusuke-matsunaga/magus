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

noinst_LTLIBRARIES = libym_logic_expr.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_logic_expr.la 用の変数指定
############################################################

libym_logic_expr_la_SOURCES = \
	lexp_types.h \
	LexpMgr.h \
	LexpNode.h \
	LexpNodePtr.h \
	LexpParser.h \
	SopLit.h \
	Literal.cc \
	LexpMgr.cc \
	LexpNode.cc \
	LexpParser.cc \
	LogExpr.cc \
	LogExprWriter.cc

libym_logic_expr_la_LIBADD =

libym_logic_expr_la_LDFLAGS =
