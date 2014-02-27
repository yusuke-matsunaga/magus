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
	libtg_iscas89.la \
	libtg_iscas89_d.la \
	libtg_iscas89_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/iscas89 \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	TgIscas89Handler.h \
	TgIscas89Handler.cc \
	TgIscas89Reader.cc


############################################################
## libtg_iscas89.la 用の変数の定義
############################################################

libtg_iscas89_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libtg_iscas89_la_SOURCES = $(COMMON_SOURCES)

libtg_iscas89_la_LIBADD =

libtg_iscas89_la_LDFLAGS =


############################################################
## libtg_iscas89_d.la 用の変数の定義
############################################################

libtg_iscas89_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libtg_iscas89_d_la_SOURCES = $(COMMON_SOURCES)

libtg_iscas89_d_la_LIBADD =

libtg_iscas89_d_la_LDFLAGS =


############################################################
## libtg_iscas89_p.la 用の変数の定義
############################################################

libtg_iscas89_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libtg_iscas89_p_la_SOURCES = $(COMMON_SOURCES)

libtg_iscas89_p_la_LIBADD =

libtg_iscas89_p_la_LDFLAGS =
