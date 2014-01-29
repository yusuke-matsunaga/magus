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

noinst_LTLIBRARIES = libym_blif.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_blif.la 用の変数の定義
############################################################

libym_blif_la_SOURCES = \
	BlifDic.h \
	BlifDic.cc \
	BlifScanner.h \
	BlifScanner.cc \
	IdCell.h \
	IdHash.h \
	IdHash.cc \
	BlifParser.h \
	BlifParser.cc \
	BlifHandler.h \
	BlifHandler.cc \
	BlifNetwork.cc \
	BlifNetworkImpl.h \
	BlifNetworkImpl.cc \
	BlifNetworkHandler.h \
	BlifNetworkHandler.cc \
	BlifNodeImpl.h \
	BlifNodeImpl.cc

libym_blif_la_LIBADD =

libym_blif_la_LDFLAGS =
