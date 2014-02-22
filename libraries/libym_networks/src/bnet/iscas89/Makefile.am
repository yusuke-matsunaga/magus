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
	libbnet_iscas89.la \
	libbnet_iscas89_d.la \
	libbnet_iscas89_p.la


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
	BNetIscas89Handler.h \
	BNetIscas89Handler.cc \
	BNetIscas89Reader.cc


############################################################
## libbnet_iscas89.la 用の変数の定義
############################################################

libbnet_iscas89_la_CXXFLAGS = -O2

libbnet_iscas89_la_SOURCES = $(COMMON_SOURCES)

libbnet_iscas89_la_LIBADD =

libbnet_iscas89_la_LDFLAGS =


############################################################
## libbnet_iscas89_d.la 用の変数の定義
############################################################

libbnet_iscas89_d_la_CXXFLAGS = -O2 -g

libbnet_iscas89_d_la_SOURCES = $(COMMON_SOURCES)

libbnet_iscas89_d_la_LIBADD =

libbnet_iscas89_d_la_LDFLAGS =


############################################################
## libbnet_iscas89_p.la 用の変数の定義
############################################################

libbnet_iscas89_p_la_CXXFLAGS = -O2 -pg

libbnet_iscas89_p_la_SOURCES = $(COMMON_SOURCES)

libbnet_iscas89_p_la_LIBADD =

libbnet_iscas89_p_la_LDFLAGS =
