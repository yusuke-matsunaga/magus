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
	pycell_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## pycell_test 用の変数の指定
############################################################

pycell_test_SOURCES = \
	pycell_test.cc

pycell_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/python/libym_cell_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/python/libym_logic_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
        $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

pycell_test_LDFLAGS = \
	$(PYTHON_LDFLAGS)
