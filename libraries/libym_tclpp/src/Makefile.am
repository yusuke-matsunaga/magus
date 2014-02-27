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
	libym_tclpp.la \
	libym_tclpp_d.la \
	libym_tclpp_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	$(TCL_INCLUDES)


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	StrBuf.h \
	TclPoptGroup.h \
	TclObj.cc \
	TclBase.cc \
	TclCmd.cc \
	TclObjMsgHandler.cc \
	TclPopt.cc \
	TclVarTrace.cc \
	TclHandler.cc


############################################################
## libym_tclpp.la 用の変数の定義
############################################################

libym_tclpp_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_tclpp_la_SOURCES = $(COMMON_SOURCES)

libym_tclpp_la_LIBADD =

libym_tclpp_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils.la \
	$(TCL_LDADD)


############################################################
## libym_tclpp_d.la 用の変数の定義
############################################################

libym_tclpp_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_tclpp_d_la_SOURCES = $(COMMON_SOURCES)

libym_tclpp_d_la_LIBADD =

libym_tclpp_d_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils_d.la \
	$(TCL_LDADD)


############################################################
## libym_tclpp_p.la 用の変数の定義
############################################################

libym_tclpp_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_tclpp_p_la_SOURCES = $(COMMON_SOURCES)

libym_tclpp_p_la_LIBADD =

libym_tclpp_p_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils_p.la \
	$(TCL_LDADD)
