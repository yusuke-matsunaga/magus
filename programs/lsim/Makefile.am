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

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	lsim


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@


############################################################
## lsim 用の変数の定義
############################################################

lsim_SOURCES = \
	lsim.cc \
	Lsim.h \
	LsimNaive.h \
	LsimNaive.cc \
	LsimNaive2.h \
	LsimNaive2.cc \
	LsimNaive3.h \
	LsimNaive3.cc \
	LsimBdd.h \
	LsimBdd.cc \
	LsimBdd2.h \
	LsimBdd2.cc \
	LsimBdd1.h \
	LsimBdd1.cc \
	LsimBdd3.h \
	LsimBdd3.cc \
	LsimBdd10.h \
	LsimBdd10.cc \
	LsimLcc.h \
	LsimLcc.cc \
	LsimMpx.h \
	LsimMpx.cc \
	LsimMpx2.h \
	LsimMpx2.cc

lsim_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

lsim_LDFLAGS = \
	@POPT_LIBS@
