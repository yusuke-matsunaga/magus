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
	libatpg_fsim3.la \
	libatpg_fsim3_d.la \
	libatpg_fsim3_p.la

noinst_PROGRAMS = \
	simnode_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	fsim3_nsdef.h \
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
	SimFFR.h \
	EventQ.h \
	EventQ.cc \
	SimFault.h \
	Fsim3.h \
	Fsim3.cc


############################################################
## libatpg_fsim3.la 用の変数の定義
############################################################

libatpg_fsim3_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_fsim3_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_fsim3_la_LIBADD =

libatpg_fsim3_la_LDFLAGS =


############################################################
## libatpg_fsim3_d.la 用の変数の定義
############################################################

libatpg_fsim3_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libatpg_fsim3_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_fsim3_d_la_LIBADD =

libatpg_fsim3_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_fsim3_p.la 用の変数の定義
############################################################

libatpg_fsim3_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libatpg_fsim3_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_fsim3_p_la_LIBADD =

libatpg_fsim3_p_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## simnode_test 用の変数の定義
############################################################

simnode_test_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

simnode_test_SOURCES = \
	simnode_test.cc

simnode_test_LDADD = \
	libatpg_fsim3_d.la

simnode_test_LDFLAGS = \
	$(AM_LDFLAGS)
