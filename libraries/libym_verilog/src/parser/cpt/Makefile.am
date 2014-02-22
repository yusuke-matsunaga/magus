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
	libvlparser_cpt.la \
	libvlparser_cpt_d.la \
	libvlparser_cpt_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/parser/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libvlparser_cpt.la 用の変数の定義
############################################################

libvlparser_cpt_la_CXXFLAGS = -O2

libvlparser_cpt_la_SOURCES = $(COMMON_SOURCES)

libvlparser_cpt_la_LIBADD =

libvlparser_cpt_la_LDFLAGS =


############################################################
## libvlparser_cpt_d.la 用の変数の定義
############################################################

libvlparser_cpt_d_la_CXXFLAGS = -O2 -g

libvlparser_cpt_d_la_SOURCES = $(COMMON_SOURCES)

libvlparser_cpt_d_la_LIBADD =

libvlparser_cpt_d_la_LDFLAGS =


############################################################
## libvlparser_cpt_p.la 用の変数の定義
############################################################

libvlparser_cpt_p_la_CXXFLAGS = -O2 -pg

libvlparser_cpt_p_la_SOURCES = $(COMMON_SOURCES)

libvlparser_cpt_p_la_LIBADD =

libvlparser_cpt_p_la_LDFLAGS =
