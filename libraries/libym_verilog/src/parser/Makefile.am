## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	include \
	main \
	scanner \
	pt_mgr \
	spt \
	cpt \
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
	libvlparser.la \
	libvlparser_d.la \
	libvlparser_p.la


############################################################
## libvlparser.la 用の変数の定義
############################################################

libvlparser_la_SOURCES =

libvlparser_la_LIBADD = \
	main/libvlparser_main.la \
	scanner/libvlparser_scanner.la \
	pt_mgr/libvlparser_pt_mgr.la \
	spt/libvlparser_spt.la \
	cpt/libvlparser_cpt.la

libvlparser_la_LDFLAGS =


############################################################
## libvlparser_d.la 用の変数の定義
############################################################

libvlparser_d_la_SOURCES =

libvlparser_d_la_LIBADD = \
	main/libvlparser_main_d.la \
	scanner/libvlparser_scanner_d.la \
	pt_mgr/libvlparser_pt_mgr_d.la \
	spt/libvlparser_spt_d.la \
	cpt/libvlparser_cpt_d.la

libvlparser_d_la_LDFLAGS =


############################################################
## libvlparser_p.la 用の変数の定義
############################################################

libvlparser_p_la_SOURCES =

libvlparser_p_la_LIBADD = \
	main/libvlparser_main_p.la \
	scanner/libvlparser_scanner_p.la \
	pt_mgr/libvlparser_pt_mgr_p.la \
	spt/libvlparser_spt_p.la \
	cpt/libvlparser_cpt_p.la

libvlparser_p_la_LDFLAGS =
