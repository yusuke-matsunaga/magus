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
	igf \
	migf \
	igf2 \
	var_analyzer \
	phf \
	phf_fixed \
	phf_fixed3 \
	phf_igu \
	phf_eval \
	phf_eval2 \
	phf_eval3


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_popt/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## igf 用の変数の定義
############################################################

igf_SOURCES = \
	igf.cc

igf_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

igf_LDFLAGS = \
	-lrt \
	$(AM_LDFLAGS)


############################################################
## mgif 用の変数の定義
############################################################

migf_SOURCES = \
	migf.cc

migf_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

migf_LDFLAGS = \
	-lrt \
	$(AM_LDFLAGS)


############################################################
## igf2 用の変数の定義
############################################################

igf2_SOURCES = \
	igf2.cc

igf2_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

igf2_LDFLAGS = \
	-lrt \
	$(AM_LDFLAGS)


############################################################
## var_analyzer 用の変数の定義
############################################################

var_analyzer_SOURCES = \
	var_analyzer.cc

var_analyzer_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

var_analyzer_LDFLAGS = \
	-lrt \
	$(AM_LDFLAGS)


############################################################
## phf 用の変数の定義
############################################################

phf_SOURCES = \
	phf.cc

phf_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## phf_fixed 用の変数の定義
############################################################

phf_fixed_SOURCES = \
	phf_fixed.cc

phf_fixed_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_fixed_LDFLAGS = \
	-lrt \
	$(AM_LDFLAGS)


############################################################
## phf_fixed3 用の変数の定義
############################################################

phf_fixed3_SOURCES = \
	phf_fixed3.cc

phf_fixed3_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_fixed3_LDFLAGS = \
	-lrt \
	$(AM_LDFLAGS)


############################################################
## phf_igu 用の変数の定義
############################################################

phf_igu_SOURCES = \
	phf_igu.cc

phf_igu_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_igu_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## phf_eval 用の変数の定義
############################################################

phf_eval_SOURCES = \
	phf_eval.cc

phf_eval_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_eval_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## phf_eval2 用の変数の定義
############################################################

phf_eval2_SOURCES = \
	phf_eval2.cc

phf_eval2_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_eval2_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## phf_eval3 用の変数の定義
############################################################

phf_eval3_SOURCES = \
	phf_eval3.cc

phf_eval3_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

phf_eval3_LDFLAGS = \
	$(AM_LDFLAGS)
