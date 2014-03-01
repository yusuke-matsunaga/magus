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

noinst_LTLIBRARIES = \
	libbb_pt.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I $(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I $(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I $(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I $(top_srcdir)/src/bb/include


############################################################
## libbb_pt.la 用の変数の定義
############################################################

libbb_pt_la_SOURCES = \
	Assign.cc \
	Eq.h \
	Lt.h \
	Le.h \
	Gt.h \
	Ge.h \
	Constr.cc \
	PtMgr.cc \
	PtNode.cc \
	PtIdNode.h \
	PtIdNode.cc \
	PtConstNode.h \
	PtConstNode.cc \
	PtUnaryOp.h \
	PtUnaryOp.cc \
	PtNegOp.h \
	PtNegOp.cc \
	PtUminusOp.h \
	PtUminusOp.cc \
	PtBinaryOp.h \
	PtBinaryOp.cc \
	PtAddOp.h \
	PtAddOp.cc \
	PtAddOp.h \
	PtAddOp.cc \
	PtSubOp.h \
	PtSubOp.cc \
	PtMulOp.h \
	PtMulOp.cc \
	PtDivOp.h \
	PtDivOp.cc \
	PtModOp.h \
	PtModOp.cc \
	PtSllOp.h \
	PtSllOp.cc \
	PtSrlOp.h \
	PtSrlOp.cc \
	PtAndOp.h \
	PtAndOp.cc \
	PtOrOp.h \
	PtOrOp.cc \
	PtXorOp.h \
	PtXorOp.cc

libbb_pt_la_LIBADD =

libbb_pt_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la
