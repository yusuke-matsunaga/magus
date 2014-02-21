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
	libym_cell_libcomp.la \
	libym_cell_libcomp_d.la \
	libym_cell_libcomp_p.la


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
	libcomp_nsdef.h \
	LcClass.h \
	LcClass.cc \
	LcGroup.h \
	LcGroup.cc \
	LcGroupMgr.h \
	LcGroupMgr.cc \
	LcLogicMgr.h \
	LcLogicMgr.cc \
	LcFFMgr.h \
	LcFFMgr.cc \
	LcLatchMgr.h \
	LcLatchMgr.cc \
	LcPatHandler.h \
	LcPatMgr.h \
	LcPatMgr.cc \
	LcPatNode.h \
	LibComp.h \
	LibComp.cc


############################################################
## libym_cell_libcomp.la 用の変数の指定
############################################################

libym_cell_libcomp_la_CXXFLAGS = -O2

libym_cell_libcomp_la_SOURCES = $(COMMON_SOURCES)

libym_cell_libcomp_la_LIBADD =

libym_cell_libcomp_la_LDFLAGS =


############################################################
## libym_cell_libcomp_d.la 用の変数の指定
############################################################

libym_cell_libcomp_d_la_CXXFLAGS = -O2 -g

libym_cell_libcomp_d_la_SOURCES = $(COMMON_SOURCES)

libym_cell_libcomp_d_la_LIBADD =

libym_cell_libcomp_d_la_LDFLAGS =


############################################################
## libym_cell_libcomp_p.la 用の変数の指定
############################################################

libym_cell_libcomp_p_la_CXXFLAGS = -O2 -pg

libym_cell_libcomp_p_la_SOURCES = $(COMMON_SOURCES)

libym_cell_libcomp_p_la_LIBADD =

libym_cell_libcomp_p_la_LDFLAGS =
