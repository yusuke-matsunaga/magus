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

bin_PROGRAMS = \
	igugen \
	igugen_d


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_popt/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## 共通の変数の定義
############################################################

COMMON_SOURCES = \
	IguGen.cc \
	igugen.cc \
	VarHeap.h \
	VarHeap.cc


############################################################
## igugen 用の変数の定義
############################################################

igugen_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

igugen_SOURCES = \
	$(COMMON_SOURCES)

igugen_LDADD = \
	../libigf/libigf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

igugen_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## igugen_d 用の変数の定義
############################################################

igugen_d_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

igugen_d_SOURCES = \
	$(COMMON_SOURCES)

igugen_d_LDADD = \
	../libigf/libigf_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_d.la

igugen_d_LDFLAGS = \
	$(AM_LDFLAGS) \
	-static
