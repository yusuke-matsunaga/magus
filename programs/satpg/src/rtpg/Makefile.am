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
	libatpg_rtpg.la \
	libatpg_rtpg_d.la \
	libatpg_rtpg_p.la


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
	RtpgImpl.h \
	RtpgImpl.cc \
	FopRtpg.h \
	FopRtpg.cc


############################################################
## libatpg_rtpg.la 用の変数の定義
############################################################

libatpg_rtpg_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_LDFLAGS)

libatpg_rtpg_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_rtpg_la_LIBADD =

libatpg_rtpg_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_rtpg_d.la 用の変数の定義
############################################################

libatpg_rtpg_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_LDFLAGS)

libatpg_rtpg_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_rtpg_d_la_LIBADD =

libatpg_rtpg_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_rtpg_p.la 用の変数の定義
############################################################

libatpg_rtpg_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_LDFLAGS)

libatpg_rtpg_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_rtpg_p_la_LIBADD =

libatpg_rtpg_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
