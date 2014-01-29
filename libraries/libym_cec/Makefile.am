## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_cec \
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

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_cec.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_cec.la 用の変数の指定
############################################################

libym_cec_la_SOURCES = \
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

libym_cec_la_LIBADD =

libym_cec_la_LDFLAGS =
