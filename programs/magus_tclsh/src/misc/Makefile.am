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

noinst_LTLIBRARIES = libmagus_misc_tcl.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## libmagus_misc_tcl.la 用の変数の定義
############################################################

libmagus_misc_tcl_la_SOURCES = \
	TimeCmd.h \
	TimeCmd.cc \
	RandCmd.h \
	RandCmd.cc \
	misc_init.cc

libmagus_misc_tcl_la_LIBADD =

libmagus_misc_tcl_la_LDFLAGS =
