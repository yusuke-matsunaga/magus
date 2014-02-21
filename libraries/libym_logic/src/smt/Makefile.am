## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	smtlibv2 \
	core \
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

noinst_LTLIBRARIES = \
	libym_logic_smt.la \
	libym_logic_smt_d.la \
	libym_logic_smt_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS =


############################################################
## libym_logic_smt.la 用の変数の指定
############################################################

libym_logic_smt_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_smt_la_SOURCES = dummy.cc

libym_logic_smt_la_LIBADD = \
	core/libym_logic_smt_core.la \
	smtlibv2/libym_logic_smt_smtlibv2.la

libym_logic_smt_la_LDFLAGS =


############################################################
## libym_logic_smt_d.la 用の変数の指定
############################################################

libym_logic_smt_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_smt_d_la_SOURCES = dummy.cc

libym_logic_smt_d_la_LIBADD = \
	core/libym_logic_smt_core_d.la \
	smtlibv2/libym_logic_smt_smtlibv2_d.la

libym_logic_smt_d_la_LDFLAGS =


############################################################
## libym_logic_smt_p.la 用の変数の指定
############################################################

libym_logic_smt_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_smt_p_la_SOURCES = dummy.cc

libym_logic_smt_p_la_LIBADD = \
	core/libym_logic_smt_core_p.la \
	smtlibv2/libym_logic_smt_smtlibv2_p.la

libym_logic_smt_p_la_LDFLAGS =
