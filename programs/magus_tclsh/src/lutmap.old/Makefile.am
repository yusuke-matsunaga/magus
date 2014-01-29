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

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libmagus_lutmap_old.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include


############################################################
## libmagus_lutmap_old.la 用の変数の定義
############################################################

libmagus_lutmap_old_la_SOURCES = \
	lutmap.h \
	SbjGraph.h \
	SbjGraph.cc \
	SbjGraph_mindepth.cc \
	bnet2sbj.cc \
	LutNetwork.h \
	LutNetwork.cc \
	lut2bnet.cc \
	EnumCutOp.h \
	EnumCutOp.cc \
	EnumCut.h \
	EnumCut.cc \
	EnumCut2.h \
	EnumCut2.cc \
	Cut.h \
	Cut.cc \
	CutHolder.h \
	CutHolder.cc \
	MapRecord.h \
	MapRecord.cc \
	CutResub.h \
	CutResub.cc \
	CrNode.h \
	CrNode.cc \
	CrHeap.h \
	CrHeap.cc \
	CrLevelQ.h \
	CrLevelQ.cc \
	PatVect.h \
	PatVect.cc \
	BoolResub.h \
	BoolResub.cc \
	ADCost.h \
	DagACover.h \
	DagACover.cc \
	TreeCover.h \
	TreeCover.cc \
	DFCover.h \
	DFCover.cc \
	WeightCover.h \
	WeightCover.cc \
	DagDCover.h \
	DagDCover.cc \
	WeightDCover.h \
	WeightDCover.cc \
	ILPGen.h \
	XDagCover.h \
	XDagCover.cc \
	CalcOdc.h \
	CalcOdc.cc \
	OdcMgr.h \
	OdcMgr.cc

libmagus_lutmap_old_la_LIBADD =

libmagus_lutmap_old_la_LDFLAGS =
	$(YM_LIBS_LIBYM_BDD) \
	$(YM_LIBS_LIBYM_BNET)
