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
	libec.la \
	libec_d.la \
	libec_p.la


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
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	EnumCut.cc \
	EnumCut2.cc \
	BottomUp.cc \
	TopDown.cc \
	TopDown2.cc \
	ZddImp.cc \
	ZddImp2.cc \
	ZddTopDown.cc


############################################################
## libec.la 用の変数の定義
############################################################

libec_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libec_la_SOURCES = \
	$(COMMON_SOURCES)

libec_la_LIBADD =

libec_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libec_d.la 用の変数の定義
############################################################

libec_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libec_d_la_SOURCES = \
	$(COMMON_SOURCES)

libec_d_la_LIBADD =

libec_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libec_p.la 用の変数の定義
############################################################

libec_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libec_p_la_SOURCES = \
	$(COMMON_SOURCES)

libec_p_la_LIBADD =

libec_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
