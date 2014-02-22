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
	libbdn_blif.la \
	libbdn_blif_d.la \
	libbdn_blif_p.la


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
	BdnBlifReader.cc \
	BlifBdnConv.h \
	BlifBdnConv.cc


############################################################
## libbdn_blif.la 用の変数の定義
############################################################

libbdn_blif_la_CXXFLAGS = -O2

libbdn_blif_la_SOURCES = $(COMMON_SOURCES)

libbdn_blif_la_LIBADD =

libbdn_blif_la_LDFLAGS =


############################################################
## libbdn_blif_d.la 用の変数の定義
############################################################

libbdn_blif_d_la_CXXFLAGS = -O2 -g

libbdn_blif_d_la_SOURCES = $(COMMON_SOURCES)

libbdn_blif_d_la_LIBADD =

libbdn_blif_d_la_LDFLAGS =


############################################################
## libbdn_blif_p.la 用の変数の定義
############################################################

libbdn_blif_p_la_CXXFLAGS = -O2 -pg

libbdn_blif_p_la_SOURCES = $(COMMON_SOURCES)

libbdn_blif_p_la_LIBADD =

libbdn_blif_p_la_LDFLAGS =
