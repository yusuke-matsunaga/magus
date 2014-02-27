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

noinst_LTLIBRARIES = libatpg_bt.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	$(GMP_INCLUDES)


############################################################
## libatpg_bt.la 用の変数の定義
############################################################

libatpg_bt_la_SOURCES = \
	BtBase.h \
	BtBase.cc \
	BtSimple.h \
	BtSimple.cc \
	BtJustBase.h \
	BtJustBase.cc \
	BtJust1.h \
	BtJust1.cc \
	BtJust2.h \
	BtJust2.cc \
	BtZdd.h \
	BtZdd.cc

libatpg_bt_la_LIBADD =

libatpg_bt_la_LDFLAGS =
