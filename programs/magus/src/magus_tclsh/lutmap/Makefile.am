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

noinst_LTLIBRARIES = libmagus_lutmap_tcl.la


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
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/techmap/include \
	-I$(top_srcdir)/src/magus_tclsh/include


############################################################
## libmagus_lutmap_tcl.la 用の変数の定義
############################################################

libmagus_lutmap_tcl_la_SOURCES = \
	Ln2BNet.h \
	Ln2BNet.cc \
	LutmapCmd.h \
	LutmapCmd.cc \
	AreaMapCmd.cc \
	DelayMapCmd.cc \
	DumpLutCmd.cc \
	Conv2BNetCmd.cc \
	WriteVqmCmd.cc

libmagus_lutmap_tcl_la_LIBADD =

libmagus_lutmap_tcl_la_LDFLAGS =
