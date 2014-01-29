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
	libphf.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(GMP_INCLUDES)


############################################################
## libphf.la 用の変数の定義
############################################################

libphf_la_SOURCES = \
	RvMgr.cc \
	IguGen.cc \
	IguGen2.cc \
	VectSetList.h \
	Variable.cc \
	PhfGraph.h \
	PhfGraph.cc \
	PhfNode.h \
	PhfEdge.h \
	PhfGen.cc \
	BmGraph.h \
	BmGraph.cc \
	BmNode.h \
	BmNode.cc \
	BmEdge.h \
	BmEdge.cc \
	IGU.cc \
	VarFunc.cc \
	XorFunc.cc \
	RandHashGen.cc

libphf_la_LIBADD =

libphf_la_LDFLAGS =
