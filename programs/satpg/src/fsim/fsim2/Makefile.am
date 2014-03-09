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
	libatpg_fsim2.la \
	libatpg_fsim2_d.la \
	libatpg_fsim2_p.la


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
	fsim2_nsdef.h \
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
	Fsim2.h \
	Fsim2.cc


############################################################
## libatpg_fsim2.la 用の変数の定義
############################################################

libatpg_fsim2_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_fsim2_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_fsim2_la_LIBADD =

libatpg_fsim2_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_fsim2_d.la 用の変数の定義
############################################################

libatpg_fsim2_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libatpg_fsim2_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_fsim2_d_la_LIBADD =

libatpg_fsim2_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_fsim2_p.la 用の変数の定義
############################################################

libatpg_fsim2_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libatpg_fsim2_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_fsim2_p_la_LIBADD =

libatpg_fsim2_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
