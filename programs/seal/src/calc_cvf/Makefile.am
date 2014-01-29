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

noinst_LTLIBRARIES = libseal_calc_cvf.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)


############################################################
## libseal_calc_cvf.la 用の変数の定義
############################################################

libseal_calc_cvf_la_SOURCES = \
	nsdef.h \
	EqElem.h \
	PoMark.h \
	PoMark.cc \
	NodeSet.h \
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
	SimFFR.h \
	SimFFR.cc \
	EventQ.h \
	EventQ.cc \
	CalcCvf.h \
	CalcCvf.cc \
	CvfCmd.h \
	CvfCmd.cc

libseal_calc_cvf_la_LIBADD = \
	$(YM_LIBS_LIBYM_TGNET) \
	$(YM_LIBS_LIBYM_LEXP) \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBYM_UTILS)

libseal_calc_cvf_la_LDFLAGS =
