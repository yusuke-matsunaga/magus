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
	init_table \
	lr


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	@POPT_INCLUDES@


############################################################
## lr 用の変数の定義
############################################################

lr_SOURCES = \
	RwtNode.h \
	RwtPat.h \
	RwtPatGen.h \
	RwtPatGen.cc \
	RwtPatList.h \
	RwtMgr.h \
	RwtMgr.cc \
	set_abc_table.cc \
	RwtOp.h \
	RwtOp.cc \
	lr.cc

lr_LDADD = \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

lr_LDFLAGS = \
	@POPT_LIBS@


############################################################
## init_table 用の変数の定義
############################################################

init_table_SOURCES = \
	init_table.cc \
	RwtNode.h \
	RwtPat.h \
	RwtPatGen.h \
	RwtPatGen.cc \
	RwtPatList.h \
	RwtMgr.h \
	RwtMgr.cc \
	set_abc_table.cc

init_table_LDADD = \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

init_table_LDFLAGS =
