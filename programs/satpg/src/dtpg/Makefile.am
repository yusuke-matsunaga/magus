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
	DtpgSat2.h \
	DtpgSat2.cc \
	DtpgSat3.h \
	DtpgSat3.cc \
	DtpgNgMgr.h \
	DtpgNgMgr.cc \
	DtpgNodeGroup.h \
	DtpgNgEdge.h \
	DtpgNodeGroup.cc \
	SatEngine.h \
	SatEngine.cc \
	SatEngineMulti.h \
	SatEngineMulti.cc \
	SatEngineMulti2.h \
	SatEngineMulti2.cc \
	SatEngineSingle.h \
	SatEngineSingle.cc \
	SatEngineSingle2.h \
	SatEngineSingle2.cc \
	LitMap.h \
	LitMap.cc


############################################################
## libatpg_dtpg.la 用の変数の定義
############################################################

libatpg_dtpg_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dtpg_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dtpg_la_LIBADD =

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

libatpg_dtpg_d_la_LIBADD =

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

libatpg_dtpg_p_la_LIBADD =

libatpg_dtpg_la_LDFLAGS = \
	$(AM_LDFLAGS)
