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

noinst_LTLIBRARIES = \
	libatpg_dtpg.la \
	libatpg_dtpg_d.la \
	libatpg_dtpg_p.la


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
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc \
	DtpgNgMgr.h \
	DtpgNgMgr.cc \
	DtpgNodeGroup.h \
	DtpgNgEdge.h \
	DtpgNodeGroup.cc \
	SatEngine.h


############################################################
## libatpg_dtpg.la 用の変数の定義
############################################################

libatpg_dtpg_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dtpg_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dtpg_la_LIBADD = \
	sat_engine/libatpg_sat_engine.la \
	bt/libatpg_bt.la \
	dop/libatpg_dop.la \
	uop/libatpg_uop.la

libatpg_dtpg_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_dtpg_d.la 用の変数の定義
############################################################

libatpg_dtpg_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dtpg_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dtpg_d_la_LIBADD = \
	sat_engine/libatpg_sat_engine_d.la \
	bt/libatpg_bt_d.la \
	dop/libatpg_dop_d.la \
	uop/libatpg_uop_d.la

libatpg_dtpg_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_dtpg_p.la 用の変数の定義
############################################################

libatpg_dtpg_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dtpg_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dtpg_p_la_LIBADD = \
	sat_engine/libatpg_sat_engine_p.la \
	bt/libatpg_bt_p.la \
	dop/libatpg_dop_p.la \
	uop/libatpg_uop_p.la

libatpg_dtpg_la_LDFLAGS = \
	$(AM_LDFLAGS)
