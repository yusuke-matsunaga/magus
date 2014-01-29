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
	libmatpg.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old_matpg/include


############################################################
## libmatpg.la 用の変数の定義
############################################################

libmatpg_la_SOURCES = \
	backtrace.cc \
	backtrace.h \
	calc_val.cc \
	cfna.cc \
	cfna.h \
	dtpg.cc \
	f_sim.cc \
	f_sim.h \
	fault.cc \
	fault.h \
	gate.cc \
	gate.h \
	imp_buf.cc \
	imp_other.cc \
	imp_sa.cc \
	imp_simple.cc \
	imp_xor.cc \
	learn.cc \
	learn.h \
	lvlq.cc \
	maqueue.cc \
	maqueue.h \
	matpg.h \
	network.cc \
	network.h \
	pgraph.cc \
	pgraph.h \
	rtpg.cc \
	sa_node.cc \
	sa_node.h \
	search_mgr.cc \
	search_mgr.h \
	sense.cc \
	sense.h \
	t_analyze.cc \
	t_analyze.h \
	testpat.cc \
	testpat.h \
	used_gate.cc \
	used_gate.h \
	val.h

libmatpg_la_LIBADD =

libmatpg_la_LDFLAGS =
