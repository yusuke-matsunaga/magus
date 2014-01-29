## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	libym_imp \
	bb \
	.


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
	imp \
	impsh


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/libym_imp \
	@GMP_INCLUDES@


############################################################
## imp 用の変数の定義
############################################################

imp_SOURCES = \
	imp.cc

imp_LDADD = \
	libym_imp/libym_imp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

imp_LDFLAGS = \
	$(POPT_LIBS) \
	$(GMP_LIBS)


############################################################
## impsh 用の変数の定義
############################################################

impsh_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	@TCL_INCLUDES@

impsh_SOURCES = \
	ImpCmd.h \
	ImpCmd.cc \
	tclAppInit.cc

impsh_LDADD = \
	libym_imp/libym_imp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

impsh_LDFLAGS = \
	$(POPT_LIBS) \
	$(GMP_LIBS)
