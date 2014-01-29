## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	smtlibv2 \
	core \
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
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_logic_smt.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_logic_smt.la 用の変数の指定
############################################################

libym_logic_smt_la_SOURCES =

libym_logic_smt_la_LIBADD = \
	core/libym_logic_smt_core.la \
	smtlibv2/libym_logic_smt_smtlibv2.la

libym_logic_smt_la_LDFLAGS =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_smt_la_SOURCES = dummy.cc
