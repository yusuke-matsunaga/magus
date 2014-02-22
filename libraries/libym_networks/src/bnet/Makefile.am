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
	libbnet.la \
	libbnet_d.la \
	libbnet_p.la


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
	StrBNodeMap.h \
	StrBNodeMap.cc \
	BNodeMgr.h \
	BNodeMgr.cc \
	BNetwork.cc \
	eliminate.cc \
	BNode.cc \
	BNetManip.cc \
	BNetDecomp.cc \
	BNetGateDecomp.cc \
	BNetSopDecomp.cc \
	BNetBlifWriter.cc \
	BNetEquWriter.cc \
	BNetVerilogWriter.cc


############################################################
## libbnet.la 用の変数の定義
############################################################

libbnet_la_CXXFLAGS = -O2

libbnet_la_SOURCES = $(COMMON_SOURCES)

libbnet_la_LIBADD = \
	blif/libbnet_blif.la \
	iscas89/libbnet_iscas89.la

libbnet_la_LDFLAGS =


############################################################
## libbnet_d.la 用の変数の定義
############################################################

libbnet_d_la_CXXFLAGS = -O2 -g

libbnet_d_la_SOURCES = $(COMMON_SOURCES)

libbnet_d_la_LIBADD = \
	blif/libbnet_blif_d.la \
	iscas89/libbnet_iscas89_d.la

libbnet_d_la_LDFLAGS =


############################################################
## libbnet_p.la 用の変数の定義
############################################################

libbnet_p_la_CXXFLAGS = -O2 -pg

libbnet_p_la_SOURCES = $(COMMON_SOURCES)

libbnet_p_la_LIBADD = \
	blif/libbnet_blif_p.la \
	iscas89/libbnet_iscas89_p.la

libbnet_p_la_LDFLAGS =
