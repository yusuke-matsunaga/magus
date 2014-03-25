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

bin_PROGRAMS = \
	rvgen \
	tseconv \
	k_of_n_gen \
	dist_sim \
	phf_sim


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_popt/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## rvgen 用の変数の定義
############################################################

rvgen_SOURCES = \
	rvgen.cc

rvgen_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/src/libym_tclpp.la

rvgen_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## tseconv 用の変数の定義
############################################################

tseconv_SOURCES = \
	tseconv.cc

tseconv_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

tseconv_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## k_of_n_gen 用の変数の定義
############################################################

k_of_n_gen_SOURCES = \
	k_of_n_gen.cc

k_of_n_gen_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

k_of_n_gen_LDFLAGS = \
	@GMP_LIBS@ \
	$(AM_LDFLAGS)


############################################################
## dist_sim 用の変数の定義
############################################################

dist_sim_SOURCES = \
	dist_sim.cc

dist_sim_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

dist_sim_LDFLAGS = \
	@GMP_LIBS@ \
	$(AM_LDFLAGS)


############################################################
## phf_sim 用の変数の定義
############################################################

phf_sim_SOURCES = \
	phf_sim.cc

phf_sim_LDADD = \
	../libigf/libigf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_sim_LDFLAGS = \
	@GMP_LIBS@ \
	$(AM_LDFLAGS)
