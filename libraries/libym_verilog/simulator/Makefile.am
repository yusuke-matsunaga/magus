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

noinst_LTLIBRARIES = libym_vl_sim.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libym_verilog


############################################################
## libym_vl_sim.la 用の変数の定義
############################################################

libym_vl_sim_la_SOURCES = \
	SimEngine.h \
	SimEventQueue.h \
	SimCaLinkQueue.h \
	SimNode.h \
	SimDelayNode.h \
	SimEventNode.h \
	SimRepeatNode.h \
	SimForkNode.h \
	SimPcaNode.h \
	SimCaLink.h \
	SimScope.h \
	SimStmtGraph.h \
	SimEngine.cc \
	SimNode.cc \
	SimDelayNode.cc \
	SimEventNode.cc \
	SimRepeatNode.cc \
	SimForkNode.cc \
	SimPcaNode.cc \
	SimCaLink.cc \
	SimScope.cc \
	SimStmtGraph.cc

libym_vl_sim_la_LIBADD =

libym_vl_sim_la_LDFLAGS =
