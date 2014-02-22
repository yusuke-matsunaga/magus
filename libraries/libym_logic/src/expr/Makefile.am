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
	libexpr.la \
	libexpr_d.la \
	libexpr_p.la


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
	LexpMgr.cc \
	LexpNode.cc \
	LexpParser.cc \
	Literal.cc \
	LogExpr.cc \
	LogExprWriter.cc \
	SopLit.h


############################################################
## libexpr.la 用の変数指定
############################################################

libexpr_la_CXXFLAGS = -O2

libexpr_la_SOURCES = $(COMMON_SOURCES)

libexpr_la_LIBADD =

libexpr_la_LDFLAGS =


############################################################
## libexpr_d.la 用の変数指定
############################################################

libexpr_d_la_CXXFLAGS = -O2 -g

libexpr_d_la_SOURCES = $(COMMON_SOURCES)

libexpr_d_la_LIBADD =

libexpr_d_la_LDFLAGS =


############################################################
## libexpr_p.la 用の変数指定
############################################################

libexpr_p_la_CXXFLAGS = -O2 -pg

libexpr_p_la_SOURCES = $(COMMON_SOURCES)

libexpr_p_la_LIBADD =

libexpr_p_la_LDFLAGS =
