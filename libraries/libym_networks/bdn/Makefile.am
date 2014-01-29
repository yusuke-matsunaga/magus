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

noinst_LTLIBRARIES = libym_bdn.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_bdn.la 用の変数の定義
############################################################

libym_bdn_la_SOURCES = \
	BdnBlifWriter.cc \
	BdnDumper.cc \
	BdnMgr.cc \
	BdnMgrImpl.h \
	BdnMgrImpl.cc \
	BdnNode1.h \
	BdnNode2.h \
	BdnNode.cc \
	BdnVerilogWriter.cc

libym_bdn_la_LIBADD = \
	blif/libym_bdn_blifreader.la \
	iscas89/libym_bdn_iscas89reader.la

libym_bdn_la_LDFLAGS =
