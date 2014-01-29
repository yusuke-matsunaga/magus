## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_logic \
	expr \
	tvfunc \
	npn \
	bdd \
	zdd \
	cnfdd \
	sat \
	smt \
	aig \
	$(YM_EXTRA_SUBDIRS) \
	. \
	tests


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

noinst_LTLIBRARIES = libym_logic.la


############################################################
## libym_logic.la 用の変数の指定
############################################################

libym_logic_la_SOURCES =

libym_logic_la_LIBADD = \
	expr/libym_logic_expr.la \
	tvfunc/libym_logic_tvfunc.la \
	npn/libym_logic_npn.la \
	bdd/libym_logic_bdd.la \
	zdd/libym_logic_zdd.la \
	cnfdd/libym_logic_cnfdd.la \
	sat/libym_logic_sat.la \
	smt/libym_logic_smt.la \
	aig/libym_logic_aig.la

libym_logic_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_la_SOURCES = dummy.cc
