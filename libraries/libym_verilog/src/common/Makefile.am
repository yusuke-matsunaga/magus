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
	libvlcommon.la \
	libvlcommon_d.la \
	libvlcommon_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/parser/include \
	-I$(top_srcdir)/src/elaborator/include \
	-I$(top_srcdir)/src/elaborator/ei


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	BitVector.cc \
	VlMgr.cc \
	VlValueRep.h \
	VlValue.cc \
	VlUdpVal.cc


############################################################
## libvlcommon.la 用の変数の定義
############################################################

libvlcommon_la_CXXFLAGS =

libvlcommon_la_SOURCES = $(COMMON_SOURCES)

libvlcommon_la_LIBADD =

libvlcommon_la_LDFLAGS =


############################################################
## libvlcommon_d.la 用の変数の定義
############################################################

libvlcommon_d_la_CXXFLAGS =

libvlcommon_d_la_SOURCES = $(COMMON_SOURCES)

libvlcommon_d_la_LIBADD =

libvlcommon_d_la_LDFLAGS =


############################################################
## libvlcommon_p.la 用の変数の定義
############################################################

libvlcommon_p_la_CXXFLAGS =

libvlcommon_p_la_SOURCES = $(COMMON_SOURCES)

libvlcommon_p_la_LIBADD =

libvlcommon_p_la_LDFLAGS =
