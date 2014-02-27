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
	libblif.la \
	libblif_d.la \
	libblif_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libblif.la 用の変数の定義
############################################################

libblif_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libblif_la_SOURCES = $(COMMON_SOURCES)

libblif_la_LIBADD =

libblif_la_LDFLAGS =


############################################################
## libblif_d.la 用の変数の定義
############################################################

libblif_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libblif_d_la_SOURCES = $(COMMON_SOURCES)

libblif_d_la_LIBADD =

libblif_d_la_LDFLAGS =


############################################################
## libblif_p.la 用の変数の定義
############################################################

libblif_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libblif_p_la_SOURCES = $(COMMON_SOURCES)

libblif_p_la_LIBADD =

libblif_p_la_LDFLAGS =
