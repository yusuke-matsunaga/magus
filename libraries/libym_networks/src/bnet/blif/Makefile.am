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
	libbnet_blif.la \
	libbnet_blif_d.la \
	libbnet_blif_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/blif \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	BNetBlifHandler.h \
	BNetBlifHandler.cc \
	BNetBlifReader.cc


############################################################
## libbnet_blif.la 用の変数の定義
############################################################

libbnet_blif_la_CXXFLAGS = -O2

libbnet_blif_la_SOURCES = $(COMMON_SOURCES)

libbnet_blif_la_LIBADD =

libbnet_blif_la_LDFLAGS =


############################################################
## libbnet_blif_d.la 用の変数の定義
############################################################

libbnet_blif_d_la_CXXFLAGS = -O2 -g

libbnet_blif_d_la_SOURCES = $(COMMON_SOURCES)

libbnet_blif_d_la_LIBADD =

libbnet_blif_d_la_LDFLAGS =


############################################################
## libbnet_blif_p.la 用の変数の定義
############################################################

libbnet_blif_p_la_CXXFLAGS = -O2 -pg

libbnet_blif_p_la_SOURCES = $(COMMON_SOURCES)

libbnet_blif_p_la_LIBADD =

libbnet_blif_p_la_LDFLAGS =
