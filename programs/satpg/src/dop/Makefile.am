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
	libatpg_dop.la \
	libatpg_dop_d.la \
	libatpg_dop_p.la



############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	DopBase.h \
	DopBase.cc \
	DopDrop.h \
	DopDrop.cc \
	FopDrop.h \
	FopDrop.cc \
	DopTvList.h \
	DopTvList.cc \
	DopVerify.h \
	DopVerify.cc \
	DopList.cc


############################################################
## libatpg_dop.la 用の変数の定義
############################################################

libatpg_dop_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dop_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dop_la_LIBADD =

libatpg_dop_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_dop_d.la 用の変数の定義
############################################################

libatpg_dop_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dop_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dop_d_la_LIBADD =

libatpg_dop_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_dop_p.la 用の変数の定義
############################################################

libatpg_dop_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libatpg_dop_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_dop_p_la_LIBADD =

libatpg_dop_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
