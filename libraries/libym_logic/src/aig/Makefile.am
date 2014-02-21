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
	libym_logic_aig.la \
	libym_logic_aig_d.la \
	libym_logic_aig_p.la


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

libym_logic_aig_la_CXXFLAGS = -O2

libym_logic_aig_la_SOURCES = $(COMMON_FLAGS)

libym_logic_aig_la_LIBADD =

libym_logic_aig_la_LDFLAGS =


############################################################
## libym_aig_d.la 用の変数の指定
############################################################

libym_logic_aig_d_la_CXXFLAGS = -O2 -g

libym_logic_aig_d_la_SOURCES = $(COMMON_FLAGS)

libym_logic_aig_d_la_LIBADD =

libym_logic_aig_d_la_LDFLAGS =


############################################################
## libym_aig_p.la 用の変数の指定
############################################################

libym_logic_aig_p_la_CXXFLAGS = -O2 -pg

libym_logic_aig_p_la_SOURCES = $(COMMON_FLAGS)

libym_logic_aig_p_la_LIBADD =

libym_logic_aig_p_la_LDFLAGS =
