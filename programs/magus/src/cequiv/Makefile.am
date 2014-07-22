## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	. \
	tests


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

noinst_LTLIBRARIES = libcequiv.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## libcequiv.la 用の変数の指定
############################################################

libcequiv_la_SOURCES = \
	FraigHash.h \
	FraigHandle.h \
	FraigNode.h \
	FraigNode.cc \
	FraigMgrImpl.h \
	FraigMgrImpl.cc \
	FraigMgr.h \
	FraigMgr.cc \
	check_ceq.cc \
	CecHandle.h \
	CecHash.h \
	CecMgr.h \
	CecMgr.cc \
	CecNode.h \
	check_ceq2.cc \
	make_miter.cc

libcequiv_la_LIBADD =

libcequiv_la_LDFLAGS =
