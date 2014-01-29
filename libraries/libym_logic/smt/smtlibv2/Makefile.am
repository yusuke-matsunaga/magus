## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	parser \
	shell \
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

noinst_LTLIBRARIES = libym_logic_smt_smtlibv2.la


############################################################
## libym_logic_smt_smtlibv2.la 用の変数の指定
############################################################

libym_logic_smt_smtlibv2_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_smt_smtlibv2_la_SOURCES = dummy.cc

libym_logic_smt_smtlibv2_la_LIBADD = \
	shell/libym_smt_shell.la \
	parser/libym_smt_parser.la

libym_logic_smt_smtlibv2_la_LDFLAGS = \
	-lreadline -lncurses
