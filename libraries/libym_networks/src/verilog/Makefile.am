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
	libvlw.la \
	libvlw_d.la \
	libvlw_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	VlWriter.h \
	VlWriter.cc \
	VlwBase.h \
	VlwBase.cc \
	VlwModule.h \
	VlwModule.cc \
	VlwModuleHeader.h \
	VlwModuleHeader.cc \
	VlwIO.h \
	VlwIO.cc \
	VlwAssign.h \
	VlwAssign.cc \
	VlwInst.h \
	VlwInst.cc


############################################################
## libvlw.la 用の変数の定義
############################################################

libvlw_la_CXXFLAGS = -O2

libvlw_la_SOURCES = $(COMMON_SOURCES)

libvlw_la_LIBADD =

libvlw_la_LDFLAGS =


############################################################
## libvlw_d.la 用の変数の定義
############################################################

libvlw_d_la_CXXFLAGS = -O2 -g

libvlw_d_la_SOURCES = $(COMMON_SOURCES)

libvlw_d_la_LIBADD =

libvlw_d_la_LDFLAGS =


############################################################
## libvlw_p.la 用の変数の定義
############################################################

libvlw_p_la_CXXFLAGS = -O2 -pg

libvlw_p_la_SOURCES = $(COMMON_SOURCES)

libvlw_p_la_LIBADD =

libvlw_p_la_LDFLAGS =
