## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLANFILES = Makefiles.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

bin_PROGRAMS = pysatpg


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(PYTHON_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old/include \
	-I$(top_srcdir)/old/src

############################################################
## pysatpg 用の変数の定義
############################################################

pysatpg_SOURCES = \
	pysatpg.cc \
	pysatpg_Val3.cc \
	pysatpg_FaultStatus.cc \
	pysatpg_SaFault.cc \
	pysatpg_FaultMgr.cc \
	pysatpg_TestVector.cc \
	pysatpg_TvMgr.cc

pysatpg_LDADD = \
	$(top_builddir)/old/src/dtpg/libatpg_dtpg.la \
	$(top_builddir)/old/src/fsim2/libatpg_fsim2.la \
	$(top_builddir)/old/src/fsim3/libatpg_fsim3.la \
	$(top_builddir)/old/src/fsimx/libatpg_fsimx.la \
	$(top_builddir)/old/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

pysatpg_LDFLAGS = \
	$(PYTHON_LDFLAGS)
