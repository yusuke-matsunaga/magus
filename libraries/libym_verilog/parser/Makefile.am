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
	libym_vl_parser.la


############################################################
## libym_vl_parser.la 用の変数の定義
############################################################

libym_vl_parser_la_SOURCES =

libym_vl_parser_la_LIBADD = \
	main/libym_vl_parser_main.la \
	scanner/libym_vl_parser_scanner.la \
	pt_mgr/libym_vl_parser_pt_mgr.la \
	spt/libym_vl_parser_spt.la \
	cpt/libym_vl_parser_cpt.la

libym_vl_parser_la_LDFLAGS =
