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

noinst_LTLIBRARIES = libym_atpg_pybase.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## libym_atpg_pybase.la 用の変数の定義
############################################################

libym_atpg_pybase_la_SOURCES = \
	pybase_init.cc \
	pybase_SaFault.cc \
	pybase_FaultMgr.cc \
	pybase_TestVector.cc

libym_atpg_pybase_la_LIBADD =

libym_atpg_pybase_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
