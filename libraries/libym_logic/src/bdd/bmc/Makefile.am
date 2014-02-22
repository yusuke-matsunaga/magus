## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIR =


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
	libbdd_bmc.la \
	libbdd_bmc_d.la \
	libbdd_bmc_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/bdd/include \
	$(GMP_INCLUDES)


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	BddMgrClassic.h \
	BddMgrClassic.cc \
	BmcCompTbl.h \
	BmcCompTbl.cc \
	bmc_ite.cc \
	bmc_isop.cc \
	bmc_onepath.cc \
	bmc_vs.cc \
	bmc_ls.cc


############################################################
## libbdd_bmc.la 用の変数の指定
############################################################

libbdd_bmc_la_CXXFLAGS = -O2

libbdd_bmc_la_SOURCES = $(COMMON_SOURCES)

libbdd_bmc_la_LIBADD =

libbdd_bmc_la_LDFLAGS =


############################################################
## libbdd_bmc_d.la 用の変数の指定
############################################################

libbdd_bmc_d_la_CXXFLAGS = -O2 -g

libbdd_bmc_d_la_SOURCES = $(COMMON_SOURCES)

libbdd_bmc_d_la_LIBADD =

libbdd_bmc_d_la_LDFLAGS =


############################################################
## libbdd_bmc_p.la 用の変数の指定
############################################################

libbdd_bmc_p_la_CXXFLAGS = -O2 -pg

libbdd_bmc_p_la_SOURCES = $(COMMON_SOURCES)

libbdd_bmc_p_la_LIBADD =

libbdd_bmc_p_la_LDFLAGS =
