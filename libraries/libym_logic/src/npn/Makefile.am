## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	utils \
	.


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
	libym_logic_npn.la \
	libym_logic_npn_d.la \
	libym_logic_npn_p.la


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
	NpnBaseConf.h \
	NpnBaseConf.cc \
	NpnRawSig.h \
	NpnRawSig.cc \
	NpnConf.h \
	NpnConf.cc \
	NpnMgr.cc \
	NpnMgrImpl.h \
	NpnMgrImpl.cc \
	NpnMap.cc \
	NpnMapM.cc


############################################################
## libym_logic_npn.la 用の変数の指定
############################################################

libym_logic_npn_la_CXXFLAGS = -O2

libym_logic_npn_la_SOURCES = $(COMMON_SOURCES)

libym_logic_npn_la_LIBADD =

libym_logic_npn_la_LDFLAGS =


############################################################
## libym_logic_npn.la 用の変数の指定
############################################################

libym_logic_npn_d_la_CXXFLAGS = -O2 -g

libym_logic_npn_d_la_SOURCES = $(COMMON_SOURCES)

libym_logic_npn_d_la_LIBADD =

libym_logic_npn_d_la_LDFLAGS =


############################################################
## libym_logic_npn_p.la 用の変数の指定
############################################################

libym_logic_npn_p_la_CXXFLAGS = -O2 -pg

libym_logic_npn_p_la_SOURCES = $(COMMON_SOURCES)

libym_logic_npn_p_la_LIBADD =

libym_logic_npn_p_la_LDFLAGS =
