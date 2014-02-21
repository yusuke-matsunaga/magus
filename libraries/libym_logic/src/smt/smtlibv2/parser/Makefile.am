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
	libym_smt_parser.la \
	libym_smt_parser_d.la \
	libym_smt_parser_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/smt/smtlibv2/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	SmtLibScanner.h \
	SmtLibScanner.cc \
	SmtLibNodeImpl.h \
	SmtLibNodeImpl.cc \
	SmtLibParser.cc


############################################################
## libym_smt_parser.la 用の変数の指定
############################################################

libym_smt_parser_la_CXXFLAGS = -O2

libym_smt_parser_la_SOURCES = $(COMMON_SOURCES)

libym_smt_parser_la_LIBADD =

libym_smt_parser_la_LDFLAGS =


############################################################
## libym_smt_parser_d.la 用の変数の指定
############################################################

libym_smt_parser_d_la_CXXFLAGS = -O2 -g

libym_smt_parser_d_la_SOURCES = $(COMMON_SOURCES)

libym_smt_parser_d_la_LIBADD =

libym_smt_parser_d_la_LDFLAGS =


############################################################
## libym_smt_parser_p.la 用の変数の指定
############################################################

libym_smt_parser_p_la_CXXFLAGS = -O2 -pg

libym_smt_parser_p_la_SOURCES = $(COMMON_SOURCES)

libym_smt_parser_p_la_LIBADD =

libym_smt_parser_p_la_LDFLAGS =
