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
	libaig.la \
	libaig_d.la \
	libaig_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	AigNode.h \
	AigNode.cc \
	AigMgr.cc \
	AigMgrImpl.h \
	AigMgrImpl.cc \
	AigSatMgr.cc

#	AigSatData.h \
#	AigSatMgr.cc \
#	AigTemplate.h \
#	AigTemplate.cc


############################################################
## libym_aig.la 用の変数の指定
############################################################

libaig_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libaig_la_SOURCES = $(COMMON_SOURCES)

libaig_la_LIBADD =

libaig_la_LDFLAGS =


############################################################
## libym_aig_d.la 用の変数の指定
############################################################

libaig_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libaig_d_la_SOURCES = $(COMMON_SOURCES)

libaig_d_la_LIBADD =

libaig_d_la_LDFLAGS =


############################################################
## libym_aig_p.la 用の変数の指定
############################################################

libaig_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libaig_p_la_SOURCES = $(COMMON_SOURCES)

libaig_p_la_LIBADD =

libaig_p_la_LDFLAGS =
