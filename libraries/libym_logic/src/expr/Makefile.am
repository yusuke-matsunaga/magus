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
	libym_logic_expr.la \
	libym_logic_expr_d.la \
	libym_logic_expr_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libym_logic_expr.la 用の変数指定
############################################################

libym_logic_expr_la_CXXFLAGS = -O2

libym_logic_expr_la_SOURCES = $(COMMON_SOURCES)

libym_logic_expr_la_LIBADD =

libym_logic_expr_la_LDFLAGS =


############################################################
## libym_logic_expr_d.la 用の変数指定
############################################################

libym_logic_expr_d_la_CXXFLAGS = -O2 -g

libym_logic_expr_d_la_SOURCES = $(COMMON_SOURCES)

libym_logic_expr_d_la_LIBADD =

libym_logic_expr_d_la_LDFLAGS =


############################################################
## libym_logic_expr_p.la 用の変数指定
############################################################

libym_logic_expr_p_la_CXXFLAGS = -O2 -pg

libym_logic_expr_p_la_SOURCES = $(COMMON_SOURCES)

libym_logic_expr_p_la_LIBADD =

libym_logic_expr_p_la_LDFLAGS =
