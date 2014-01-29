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

noinst_LTLIBRARIES = \
	libym_smt_parser.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/smt/smtlibv2/include


############################################################
## libym_smt_parser.la 用の変数の指定
############################################################

libym_smt_parser_la_SOURCES = \
	SmtLibScanner.h \
	SmtLibScanner.cc \
	SmtLibNodeImpl.h \
	SmtLibNodeImpl.cc \
	SmtLibParser.cc

libym_smt_parser_la_LIBADD =

libym_smt_parser_la_LDFLAGS =
