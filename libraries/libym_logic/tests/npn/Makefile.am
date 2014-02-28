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
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	$(TCL_INCLUDES)


############################################################
## 共通の変数の定義
############################################################


############################################################
## tvfunc_test 用の変数の指定
############################################################

tvfunc_test_SOURCES = \
	TvFuncTest.h \
	TvFuncTest.cc \
	tvfunc_test.cc

tvfunc_test_LDADD = \
	$(top_builddir)/src/libym_logic.la \
	$(top_builddir)/../libym_popt/src/libym_popt.la

tvfunc_test_LDFLAGS =


############################################################
## tv_func_test2 用の変数の指定
############################################################

tvfunc_test2_SOURCES = \
	tvfunc_test2.cc

tvfunc_test2_LDADD = \
	$(top_builddir)/src/libym_logic.la \
	$(top_builddir)/../libym_popt/src/libym_popt.la

tvfunc_test2_LDFLAGS =


############################################################
## cofactor_test 用の変数の指定
############################################################

cofactor_test_SOURCES = \
	cofactor_test.cc \
	TvFuncTest.h \
	TvFuncTest.cc

cofactor_test_LDADD = \
	$(top_builddir)/src/libym_logic.la

cofactor_test_LDFLAGS =


############################################################
## npn_check3 用の変数の指定
############################################################

npn_check3_SOURCES = \
	npn_check3.cc

npn_check3_LDADD = \
	$(top_builddir)/src/libym_logic.la

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
