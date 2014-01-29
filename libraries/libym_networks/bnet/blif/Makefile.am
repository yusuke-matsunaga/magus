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

noinst_LTLIBRARIES = libym_bnetblifreader.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/blif


############################################################
## libym_bnetblifreader.la 用の変数の定義
############################################################

libym_bnetblifreader_la_SOURCES = \
	BNetBlifHandler.h \
	BNetBlifHandler.cc \
	BNetBlifReader.cc

libym_bnetblifreader_la_LIBADD =

libym_bnetblifreader_la_LDFLAGS =
