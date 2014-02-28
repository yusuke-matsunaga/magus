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
	libbdd.la \
	libbdd_d.la \
	libbdd_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS =


############################################################
## libbdd.la 用の変数の指定
############################################################

libbdd_la_SOURCES =

nodist_EXTRA_libbdd_la_SOURCES = dummy.cc

libbdd_la_LIBADD = \
	base/libbdd_base.la \
	bmc/libbdd_bmc.la \
	bmm/libbdd_bmm.la

libbdd_la_LDFLAGS =


############################################################
## libbdd_d.la 用の変数の指定
############################################################

libbdd_d_la_SOURCES =

nodist_EXTRA_libbdd_d_la_SOURCES = dummy.cc

libbdd_d_la_LIBADD = \
	base/libbdd_base_d.la \
	bmc/libbdd_bmc_d.la \
	bmm/libbdd_bmm_d.la

libbdd_d_la_LDFLAGS =


############################################################
## libbdd_p.la 用の変数の指定
############################################################

libbdd_p_la_SOURCES =

nodist_EXTRA_libbdd_p_la_SOURCES = dummy.cc

libbdd_p_la_LIBADD = \
	base/libbdd_base_p.la \
	bmc/libbdd_bmc_p.la \
	bmm/libbdd_bmm_p.la

libbdd_p_la_LDFLAGS =
