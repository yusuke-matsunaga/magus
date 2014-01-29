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
	libigf_python.la

noinst_PROGRAMS = \
	pyigf_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## libigf_python.la 用の変数の定義
############################################################

libigf_python_la_SOURCES = \
	pyigf_FuncVect.cc \
	pyigf_PhfGen.cc \
	pyigf_RegVect.cc \
	pyigf_RvMgr.cc \
	pyigf_init.cc

libigf_python_la_LIBADD =

libigf_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)


############################################################
## pyigf_test 用の変数の定義
############################################################

pyigf_test_SOURCES = \
	pyigf_test.cc

pyigf_test_LDADD = \
	libigf_python.la \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pyigf_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
