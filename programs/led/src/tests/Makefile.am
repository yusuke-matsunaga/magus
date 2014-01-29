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
	symbol_test \
	gateobj_test \
	gatemgr_test \
	networkview_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(QT_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/libled


############################################################
## symbol_test 用の変数の定義
############################################################

symbol_test_SOURCES = \
	symbol_test.cc

symbol_test_LDADD = \
	../libled/libled.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

symbol_test_LDFLAGS = \
	$(QT_LIBS)


############################################################
## gateobj_test 用の変数の定義
############################################################

gateobj_test_SOURCES = \
	gateobj_test.cc

gateobj_test_LDADD = \
	../libled/libled.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

gateobj_test_LDFLAGS = \
	$(QT_LIBS)


############################################################
## gatemgr_test 用の変数の定義
############################################################

gatemgr_test_SOURCES = \
	gatemgr_test.cc

gatemgr_test_LDADD = \
	../libled/libled.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

gatemgr_test_LDFLAGS = \
	$(QT_LIBS)


############################################################
## networkview_test 用の変数の定義
############################################################

networkview_test_SOURCES = \
	networkview_test.cc

networkview_test_LDADD = \
	../libled/libled.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

networkview_test_LDFLAGS = \
	$(QT_LIBS)
