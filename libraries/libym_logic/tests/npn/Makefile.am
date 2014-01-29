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

noinst_PROGRAMS = \
	tvfunc_test \
	tvfunc_test2 \
	cofactor_test \
	npn_check3 \
	$(NPN_CHECK)


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(TCL_INCLUDES) \
	@POPT_INCLUDES@


############################################################
## 共通の変数の定義
############################################################

YM_TCLPP = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

YM_LOGIC = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## tvfunc_test 用の変数の指定
############################################################

tvfunc_test_SOURCES = \
	TvFuncTest.h \
	TvFuncTest.cc \
	tvfunc_test.cc

tvfunc_test_LDADD = \
	$(YM_LOGIC) \
	@POPT_LIBS@

tvfunc_test_LDFLAGS =


############################################################
## tv_func_test2 用の変数の指定
############################################################

tvfunc_test2_SOURCES = \
	tvfunc_test2.cc

tvfunc_test2_LDADD = \
	$(YM_LOGIC) \
	@POPT_LIBS@

tvfunc_test2_LDFLAGS =


############################################################
## cofactor_test 用の変数の指定
############################################################

cofactor_test_SOURCES = \
	cofactor_test.cc \
	TvFuncTest.h \
	TvFuncTest.cc

cofactor_test_LDADD = \
	$(YM_LOGIC)

cofactor_test_LDFLAGS =


############################################################
## npn_check3 用の変数の指定
############################################################

npn_check3_SOURCES = \
	npn_check3.cc

npn_check3_LDADD = \
	$(YM_LOGIC) \
	@POPT_LIBS@

npn_check3_LDFLAGS =

#npn_check_SOURCES = \
#	TvFuncTest.h \
#	TvFuncTest.cc \
#	TvFuncConv.h \
#	TvFuncConv.cc \
#	TvFuncTestCmd.h \
#	TvFuncTestCmd.cc \
#	TvFuncTimeCmd.h \
#	TvFuncTimeCmd.cc \
#	FuncCmd.h \
#	FuncCmd.cc \
#	npn_check.cc
#npn_check_LDADD = \
#	$(YM_LOGIC) \
#	$(YM_TCLPP) \
#	$(YM_BASE)
