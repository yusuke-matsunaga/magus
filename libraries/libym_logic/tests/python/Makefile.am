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
	pylogic_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## pylogic_test 用の変数の指定
############################################################

pylogic_test_SOURCES = \
	pylogic_test.cc

pylogic_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/python/libym_logic_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pylogic_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
