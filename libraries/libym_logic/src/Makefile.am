## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	expr \
	tvfunc \
	npn \
	bdd \
	zdd \
	cnfdd \
	sat \
	smt \
	aig


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
	libym_logic.la \
	libym_logic_d.la \
	libym_logic_p.la


############################################################
## libym_logic.la 用の変数の指定
############################################################

libym_logic_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_la_SOURCES = dummy.cc

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
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la


############################################################
## libym_logic_d.la 用の変数の指定
############################################################

libym_logic_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_d_la_SOURCES = dummy.cc

libym_logic_d_la_LIBADD = \
	expr/libym_logic_expr_d.la \
	tvfunc/libym_logic_tvfunc_d.la \
	npn/libym_logic_npn_d.la \
	bdd/libym_logic_bdd_d.la \
	zdd/libym_logic_zdd_d.la \
	cnfdd/libym_logic_cnfdd_d.la \
	sat/libym_logic_sat_d.la \
	smt/libym_logic_smt_d.la \
	aig/libym_logic_aig_d.la

libym_logic_d_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_d.la


############################################################
## libym_logic_p.la 用の変数の指定
############################################################

libym_logic_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_p_la_SOURCES = dummy.cc

libym_logic_p_la_LIBADD = \
	expr/libym_logic_expr_p.la \
	tvfunc/libym_logic_tvfunc_p.la \
	npn/libym_logic_npn_p.la \
	bdd/libym_logic_bdd_p.la \
	zdd/libym_logic_zdd_p.la \
	cnfdd/libym_logic_cnfdd_p.la \
	sat/libym_logic_sat_p.la \
	smt/libym_logic_smt_p.la \
	aig/libym_logic_aig_p.la

libym_logic_p_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_p.la
