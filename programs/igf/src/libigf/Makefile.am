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

lib_LTLIBRARIES = \
	libigf.la \
	libigf_d.la \
	libigf_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	$(GMP_INCLUDES)


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	RvMgr.cc \
	Variable.cc \
	PhfGraph.h \
	PhfGraph.cc \
	PhfNode.h \
	PhfEdge.h \
	PhfGen.cc \
	IGU.cc \
	IguGen.cc \
	VarFunc.cc \
	XorFunc.cc \
	RandHashGen.cc


############################################################
## libigf.la 用の変数の定義
############################################################

libigf_la_CXXFLAGS = \
	$(YM_CXXOPFLAGS) \
	$(AM_CXXFLAGS)

libigf_la_SOURCES = \
	$(COMMON_SOURCES)

libigf_la_LIBADD =

libigf_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la \
	$(AM_LDFLAGS)


############################################################
## libigf_d.la 用の変数の定義
############################################################

libigf_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libigf_d_la_SOURCES = \
	$(COMMON_SOURCES)

libigf_d_la_LIBADD =

libigf_d_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_d.la \
	$(AM_LDFLAGS)


############################################################
## libigf_p.la 用の変数の定義
############################################################

libigf_p_la_CXXFLAGS = \
	$(YM_CXXPRFLAGS) \
	$(AM_CXXFLAGS)

libigf_p_la_SOURCES = \
	$(COMMON_SOURCES)

libigf_p_la_LIBADD =

libigf_p_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_p.la \
	$(AM_LDFLAGS)
