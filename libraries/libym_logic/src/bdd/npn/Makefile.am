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

noinst_LTLIBRARIES = libym_bdd_npn.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	$(GMP_INCLUDES)


############################################################
## libym_bdd_npn.la 用の変数の指定
############################################################

libym_bdd_npn_la_SOURCES = \
	WalshSig.cc \
	WalshSig.h \
	FuncEntry.h \
	RepInfo.cc \
	RepInfo.h \
	RepFunc.cc \
	RepFunc.h \
	FuncGroup.cc \
	FuncGroup.h \
	Matcher.cc \
	Matcher.h \
	NpnMap.cc \
	NpnMgr.cc

libym_bdd_npn_la_LIBADD =

libym_bdd_npn_la_LDFLAGS =
