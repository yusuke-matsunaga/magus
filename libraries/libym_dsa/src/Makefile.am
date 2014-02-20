## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	btg \
	mfset


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

lib_LTLIBRARIES = libym_dsa.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## libym_dsa.la 用の変数の定義
############################################################

libym_dsa_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_dsa_la_SOURCES = dummy.cc

libym_dsa_la_LIBADD = \
	btg/libbtg.la \
	mfset/libmfset.la

libym_dsa_la_LDFLAGS =
