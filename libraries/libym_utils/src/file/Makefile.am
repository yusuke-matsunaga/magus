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
	libfile.la \
	libfile_d.la \
	libfile_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	File.cc \
	FileInfo.h \
	FileInfo.cc \
	FileInfoMgr.h \
	FileInfoMgr.cc


############################################################
## libfile.la 用の変数の定義
############################################################

libfile_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libfile_la_SOURCES = $(COMMON_SOURCES)

libfile_la_LIBADD =

libfile_la_LDFLAGS =


############################################################
## libfile_d.la 用の変数の定義
############################################################

libfile_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libfile_d_la_SOURCES = $(COMMON_SOURCES)

libfile_d_la_LIBADD =

libfile_d_la_LDFLAGS =


############################################################
## libfile_p.la 用の変数の定義
############################################################

libfile_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libfile_p_la_SOURCES = $(COMMON_SOURCES)

libfile_p_la_LIBADD =

libfile_p_la_LDFLAGS =
