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
	libatpg_main.la \
	libatpg_main_d.la \
	libatpg_main_p.la


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
	AtpgMgr.cc


############################################################
## libatpg_main.la 用の変数の定義
############################################################

libatpg_main_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_main_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_main_la_LIBADD =

libatpg_main_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_main_d.la 用の変数の定義
############################################################

libatpg_main_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libatpg_main_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_main_d_la_LIBADD =

libatpg_main_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_main_p.la 用の変数の定義
############################################################

libatpg_main_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libatpg_main_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_main_p_la_LIBADD =

libatpg_main_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
