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

noinst_LTLIBRARIES = libym_bnetiscas89reader.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/iscas89


############################################################
## libym_bnetiscas89reader.la 用の変数の定義
############################################################

libym_bnetiscas89reader_la_SOURCES = \
	BNetIscas89Handler.h \
	BNetIscas89Handler.cc \
	BNetIscas89Reader.cc

libym_bnetiscas89reader_la_LIBADD =

libym_bnetiscas89reader_la_LDFLAGS =
