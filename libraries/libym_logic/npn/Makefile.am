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

noinst_LTLIBRARIES = libym_logic_npn.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_logic_npn.ls 用の変数の指定
############################################################

libym_logic_npn_la_SOURCES = \
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

libym_logic_npn_la_LIBADD =

libym_logic_npn_la_LDFLAGS =
