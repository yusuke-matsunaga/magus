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

noinst_PROGRAMS = \
	pyutils_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## Variables for building pyutils_test
############################################################

pyutils_test_SOURCES = \
	pyutils_test.cc

pyutils_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pyutils_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
