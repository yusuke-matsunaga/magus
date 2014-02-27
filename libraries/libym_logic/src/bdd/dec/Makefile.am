## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIR =


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
	libym_bdd_dec.la \
	libym_bdd_dec_d.la \
	libym_bdd_dec_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	$(GMP_INCLUDES)


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	Dg.cc \
	DgNode.cc \
	DgNode.h \
	DgMgr.cc \
	Bidec.cc \
	PrimDec.cc


############################################################
## libym_bdd_dec.la 用の変数の指定
############################################################

libym_bdd_dec_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_bdd_dec_la_SOURCES = $(COMMON_SOURCES)

libym_bdd_dec_la_LIBADD =

libym_bdd_dec_la_LDFLAGS =


############################################################
## libym_bdd_dec_d.la 用の変数の指定
############################################################

libym_bdd_dec_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_bdd_dec_d_la_SOURCES = $(COMMON_SOURCES)

libym_bdd_dec_d_la_LIBADD =

libym_bdd_dec_d_la_LDFLAGS =


############################################################
## libym_bdd_dec_p.la 用の変数の指定
############################################################

libym_bdd_dec_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_bdd_dec_p_la_SOURCES = $(COMMON_SOURCES)

libym_bdd_dec_p_la_LIBADD =

libym_bdd_dec_p_la_LDFLAGS =
