## Process this file with automake to produce Makefile.in


############################################################
## autotroll のおまじない
############################################################

include $(top_srcdir)/../../etc/autotroll.mk


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
	libled.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(QT_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## libled.la 用の変数の定義
############################################################

libled_la_SOURCES = \
	DrawObj.h \
	DrawObj.cc \
	Symbol.h \
	Symbol.cc \
	GateColor.h \
	GateColor.cc \
	GateMgr.h \
	GateMgr.cc \
	GateObj.cc \
	NetworkView.cc \
	NetworkView.moc.cc

BUILT_SOURCES = \
	NetworkView.moc.cc

libled_la_LIBADD =

libled_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(QT_LIBS)

NetworkView.moc.cc : $(top_srcdir)/include/NetworkView.h
