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
	libvlparser_spt.la \
	libvlparser_spt_d.la \
	libvlparser_spt_p.la


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


############################################################
## libvlparser_spt.la 用の変数の定義
############################################################

libvlparser_spt_la_CXXFLAGS = -O2

libvlparser_spt_la_SOURCES = $(COMMON_SOURCES)

libvlparser_spt_la_LIBADD =

libvlparser_spt_la_LDFLAGS =


############################################################
## libvlparser_spt_d.la 用の変数の定義
############################################################

libvlparser_spt_d_la_CXXFLAGS = -O2 -g

libvlparser_spt_d_la_SOURCES = $(COMMON_SOURCES)

libvlparser_spt_d_la_LIBADD =

libvlparser_spt_d_la_LDFLAGS =


############################################################
## libvlparser_spt_p.la 用の変数の定義
############################################################

libvlparser_spt_p_la_CXXFLAGS = -O2 -pg

libvlparser_spt_p_la_SOURCES = $(COMMON_SOURCES)

libvlparser_spt_p_la_LIBADD =

libvlparser_spt_p_la_LDFLAGS =
