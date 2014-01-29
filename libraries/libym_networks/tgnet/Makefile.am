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

noinst_LTLIBRARIES = libym_tgnet.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_tgnet.la 用の変数の定義
############################################################

libym_tgnet_la_SOURCES = \
	NameHash.h \
	NameHash.cc \
	LogicMgr.h \
	LogicMgr.cc \
	TgNetwork.cc

libym_tgnet_la_LIBADD = \
	blif/libym_tgblifreader.la \
	iscas89/libym_tgiscas89reader.la

libym_tgnet_la_LDFLAGS =
