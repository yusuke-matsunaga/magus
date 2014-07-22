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

noinst_LTLIBRARIES = libmagus_seal_tcl.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_tclpp/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## libmagus_seql_tcl.la 用の変数の定義
############################################################

libmagus_seal_tcl_la_SOURCES = \
	SealCmd.h \
	SealCmd.cc \
	TMRCmd.h \
	TMRCmd.cc \
	DupCmd.h \
	DupCmd.cc \
	McaCmd.h \
	McaCmd.cc \
	McaCmd2.h \
	McaCmd2.cc \
	seal_init.cc

libmagus_seal_tcl_la_LIBADD =

libmagus_seal_tcl_la_LDFLAGS =
