## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	aig \
	bdd \
	cnfdd \
	expr \
	npn \
	sat \
	smt \
	tvfunc \
	zdd


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
## 共通の変数定義
############################################################

utils_dir = $(top_builddir)/../libym_utils/src


############################################################
## libym_logic.la 用の変数の指定
############################################################

libym_logic_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_la_SOURCES = dummy.cc

libym_logic_la_LIBADD = \
	aig/libaig.la \
	bdd/libbdd.la \
	cnfdd/libcnfdd.la \
	expr/libexpr.la \
	npn/libnpn.la \
	sat/libsat.la \
	smt/libsmt.la \
	tvfunc/libtvfunc.la \
	zdd/libzdd.la

libym_logic_la_LDFLAGS = \
	$(utils_dir)/libym_utils.la \
	$(GMP_LIBS)


############################################################
## libym_logic_d.la 用の変数の指定
############################################################

libym_logic_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_d_la_SOURCES = dummy.cc

libym_logic_d_la_LIBADD = \
	aig/libaig_d.la \
	bdd/libbdd_d.la \
	cnfdd/libcnfdd_d.la \
	expr/libexpr_d.la \
	npn/libnpn_d.la \
	sat/libsat_d.la \
	smt/libsmt_d.la \
	tvfunc/libtvfunc_d.la \
	zdd/libzdd_d.la

libym_logic_d_la_LDFLAGS = \
	$(utils_dir)/libym_utils_d.la \
	$(GMP_LIBS)


############################################################
## libym_logic_p.la 用の変数の指定
############################################################

libym_logic_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_p_la_SOURCES = dummy.cc

libym_logic_p_la_LIBADD = \
	aig/libaig_p.la \
	bdd/libbdd_p.la \
	cnfdd/libcnfdd_p.la \
	expr/libexpr_p.la \
	npn/libnpn_p.la \
	sat/libsat_p.la \
	smt/libsmt_p.la \
	tvfunc/libtvfunc_p.la \
	zdd/libzdd_p.la

libym_logic_p_la_LDFLAGS = \
	$(utils_dir)/libym_utils_p.la \
	$(GMP_LIBS)
