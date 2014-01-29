## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_verilog \
	common \
	parser \
	elaborator \
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

noinst_LTLIBRARIES = libym_verilog.la


############################################################
## libym_verilog.la 用の変数の定義
############################################################

libym_verilog_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_verilog_la_SOURCES = dummy.cc

libym_verilog_la_LIBADD = \
	common/libym_vl_common.la \
	parser/libym_vl_parser.la \
	elaborator/libym_vl_elab.la

libym_verilog_la_LDFLAGS =
