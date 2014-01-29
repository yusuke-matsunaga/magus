## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	include \
	parser \
	pt \
	.

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
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/bb/include


############################################################
## build ターゲットの指定
############################################################

bin_PROGRAMS = \
	bb


############################################################
## bb 用の変数の定義
############################################################

bb_SOURCES = \
	bb.cc

bb_LDADD = \
	../libym_imp/libym_imp.la \
	parser/libbb_parser.la \
	pt/libbb_pt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la

bb_LDFLAGS =
