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

noinst_LTLIBRARIES = libmagus_bnet_tcl.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## libmagus_bnet_tcl.la 用の変数の定義
############################################################

libmagus_bnet_tcl_la_SOURCES = \
	BNetCmd.cc \
	BNetNodeCmd.h \
	BNetNodeCmd.cc \
	BNetInfoCmd.h \
	BNetInfoCmd.cc \
	BNetIoCmd.h \
	BNetIoCmd.cc \
	BNetNdCmd.h \
	BNetNdCmd.cc \
	BNetElimCmd.h \
	BNetElimCmd.cc \
	bnet_init.cc

libmagus_bnet_tcl_la_LIBADD =

libmagus_bnet_tcl_la_LDFLAGS =
