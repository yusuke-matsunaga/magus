## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	blif \
	iscas89 \
	.

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
	libtgnet.la \
	libtgnet_d.la \
	libtgnet_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	NameHash.h \
	NameHash.cc \
	LogicMgr.h \
	LogicMgr.cc \
	TgNetwork.cc


############################################################
## libtgnet.la 用の変数の定義
############################################################

libtgnet_la_CXXFLAGS = -O2

libtgnet_la_SOURCES = $(COMMON_SOURCES)

libtgnet_la_LIBADD = \
	blif/libtg_blif.la \
	iscas89/libtg_iscas89.la

libtgnet_la_LDFLAGS =


############################################################
## libtgnet_d.la 用の変数の定義
############################################################

libtgnet_d_la_CXXFLAGS = -O2 -g

libtgnet_d_la_SOURCES = $(COMMON_SOURCES)

libtgnet_d_la_LIBADD = \
	blif/libtg_blif_d.la \
	iscas89/libtg_iscas89_d.la

libtgnet_d_la_LDFLAGS =


############################################################
## libtgnet_p.la 用の変数の定義
############################################################

libtgnet_p_la_CXXFLAGS = -O2 -pg

libtgnet_p_la_SOURCES = $(COMMON_SOURCES)

libtgnet_p_la_LIBADD = \
	blif/libtg_blif_p.la \
	iscas89/libtg_iscas89_p.la

libtgnet_p_la_LDFLAGS =
