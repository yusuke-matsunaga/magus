## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	bb_grammer.cc bb_grammer.hh y.output bb_grammer.output \
	bblex.cc

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = \
	libbb_parser.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I $(YMTOOLS_SRCDIR)/include \
	-I $(YMTOOLS_BUILDDIR)/include \
	-I $(top_srcdir)/src/bb/include

AM_YFLAGS = -d -v


############################################################
## libbb_parser.la 用の変数の定義
############################################################

libbb_parser_la_SOURCES = \
	bb_grammer.yy \
	Lexer.h \
	Lexer.cc \
	Driver.cc

libbb_parser_la_LIBADD =

libbb_parser_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
