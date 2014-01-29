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

noinst_LTLIBRARIES = libatpg_fsim.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old_atpg/include


############################################################
## libatpg_fsim.la 用の変数の定義
############################################################

libatpg_fsim_la_SOURCES = \
	EqElem.h \
	SimNode.h \
	SimNode.cc \
	SnGate.h \
	SnGate.cc \
	SnInput.h \
	SnInput.cc \
	SnAnd.h \
	SnAnd.cc \
	SnOr.h \
	SnOr.cc \
	SnXor.h \
	SnXor.cc \
	SnCplx.h \
	SnCplx.cc \
	SimFFR.h \
	EventQ.h \
	EventQ.cc \
	FsimFault.h \
	Fsim.h \
	Fsim.cc

libatpg_fsim_la_LIBADD =

libatpg_fsim_la_LDFLAGS =
