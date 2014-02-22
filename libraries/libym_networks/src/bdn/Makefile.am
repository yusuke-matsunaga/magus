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
	 libbdn.la \
	 libbdn_d.la \
	 libbdn_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/verilog \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	BdnBlifWriter.cc \
	BdnDumper.cc \
	BdnMgr.cc \
	BdnMgrImpl.h \
	BdnMgrImpl.cc \
	BdnNode1.h \
	BdnNode2.h \
	BdnNode.cc \
	BdnVerilogWriter.cc


############################################################
## libbdn.la 用の変数の定義
############################################################

libbdn_la_CXXFLAGS = -O2

libbdn_la_SOURCES = $(COMMON_SOURCES)

libbdn_la_LIBADD = \
	blif/libbdn_blif.la \
	iscas89/libbdn_iscas89.la

libbdn_la_LDFLAGS =


############################################################
## libbdn_d.la 用の変数の定義
############################################################

libbdn_d_la_CXXFLAGS = -O2 -g

libbdn_d_la_SOURCES = $(COMMON_SOURCES)

libbdn_d_la_LIBADD = \
	blif/libbdn_blif_d.la \
	iscas89/libbdn_iscas89_d.la

libbdn_d_la_LDFLAGS =


############################################################
## libbdn_p.la 用の変数の定義
############################################################

libbdn_p_la_CXXFLAGS = -O2 -pg

libbdn_p_la_SOURCES = $(COMMON_SOURCES)

libbdn_p_la_LIBADD = \
	blif/libbdn_blif_p.la \
	iscas89/libbdn_iscas89_p.la

libbdn_p_la_LDFLAGS =
