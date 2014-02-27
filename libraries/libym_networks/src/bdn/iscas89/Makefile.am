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
	libbdn_iscas89.la \
	libbdn_iscas89_d.la \
	libbdn_iscas89_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/iscas89 \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	Iscas89BdnConv.h \
	Iscas89BdnConv.cc \
	BdnIscas89Reader.cc


############################################################
## libbdn_iscas89.la 用の変数の定義
############################################################

libbdn_iscas89_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libbdn_iscas89_la_SOURCES = $(COMMON_SOURCES)

libbdn_iscas89_la_LIBADD =

libbdn_iscas89_la_LDFLAGS =


############################################################
## libbdn_iscas89_d.la 用の変数の定義
############################################################

libbdn_iscas89_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libbdn_iscas89_d_la_SOURCES = $(COMMON_SOURCES)

libbdn_iscas89_d_la_LIBADD =

libbdn_iscas89_d_la_LDFLAGS =


############################################################
## libbdn_iscas89_p.la 用の変数の定義
############################################################

libbdn_iscas89_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libbdn_iscas89_p_la_SOURCES = $(COMMON_SOURCES)

libbdn_iscas89_p_la_LIBADD =

libbdn_iscas89_p_la_LDFLAGS =
