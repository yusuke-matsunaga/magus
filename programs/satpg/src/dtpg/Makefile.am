## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	sat_engine \
	bt \
	dop \
	uop


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

noinst_LTLIBRARIES = libatpg_dtpg.la


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
	-I$(top_srcdir)/src \
	$(GMP_INCLUDES)


############################################################
## libatpg_dtpg.la 用の変数の定義
############################################################

libatpg_dtpg_la_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc \
	DtpgNgMgr.h \
	DtpgNgMgr.cc \
	DtpgNodeGroup.h \
	DtpgNgEdge.h \
	DtpgNodeGroup.cc \
	SatEngine.h

libatpg_dtpg_la_LIBADD = \
	sat_engine/libatpg_sat_engine.la \
	bt/libatpg_bt.la \
	dop/libatpg_dop.la \
	uop/libatpg_uop.la

libatpg_dtpg_la_LDFLAGS =
