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
	libnpn.la \
	libnpn_d.la \
	libnpn_p.la


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
## libnpn.la 用の変数の指定
############################################################

libnpn_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libnpn_la_SOURCES = $(COMMON_SOURCES)

libnpn_la_LIBADD =

libnpn_la_LDFLAGS =


############################################################
## libnpn.la 用の変数の指定
############################################################

libnpn_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libnpn_d_la_SOURCES = $(COMMON_SOURCES)

libnpn_d_la_LIBADD =

libnpn_d_la_LDFLAGS =


############################################################
## libnpn_p.la 用の変数の指定
############################################################

libnpn_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libnpn_p_la_SOURCES = $(COMMON_SOURCES)

libnpn_p_la_LIBADD =

libnpn_p_la_LDFLAGS =
