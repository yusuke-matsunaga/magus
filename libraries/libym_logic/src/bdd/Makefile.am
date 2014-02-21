## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	include \
	base \
	bmc \
	bmm \
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
	libym_logic_bdd.la \
	libym_logic_bdd_d.la \
	libym_logic_bdd_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS =


############################################################
## libym_logic_bdd.la 用の変数の指定
############################################################

libym_logic_bdd_la_SOURCES =

nodist_EXTRA_libym_logic_bdd_la_SOURCES = dummy.cc

libym_logic_bdd_la_LIBADD = \
	base/libym_bdd_base.la \
	bmc/libym_bdd_bmc.la \
	bmm/libym_bdd_bmm.la

libym_logic_bdd_la_LDFLAGS =


############################################################
## libym_logic_bdd_d.la 用の変数の指定
############################################################

libym_logic_bdd_d_la_SOURCES =

nodist_EXTRA_libym_logic_bdd_d_la_SOURCES = dummy.cc

libym_logic_bdd_d_la_LIBADD = \
	base/libym_bdd_base_d.la \
	bmc/libym_bdd_bmc_d.la \
	bmm/libym_bdd_bmm_d.la

libym_logic_bdd_la_LDFLAGS =


############################################################
## libym_logic_bdd_p.la 用の変数の指定
############################################################

libym_logic_bdd_p_la_SOURCES =

nodist_EXTRA_libym_logic_bdd_p_la_SOURCES = dummy.cc

libym_logic_bdd_p_la_LIBADD = \
	base/libym_bdd_base_p.la \
	bmc/libym_bdd_bmc_p.la \
	bmm/libym_bdd_bmm_p.la

libym_logic_bdd_p_la_LDFLAGS =
