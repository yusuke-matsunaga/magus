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

noinst_LTLIBRARIES = libym_vl_common.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/parser/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/elaborator/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/elaborator/ei


############################################################
## libym_vl_common.la 用の変数の定義
############################################################

libym_vl_common_la_SOURCES = \
	BitVector.cc \
	VlMgr.cc \
	VlValueRep.h \
	VlValue.cc \
	VlUdpVal.cc

libym_vl_common_la_LIBADD =

libym_vl_common_la_LDFLAGS =
