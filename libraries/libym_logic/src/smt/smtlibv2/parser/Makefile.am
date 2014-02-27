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
	libsmtparser.la \
	libsmtparser_d.la \
	libsmtparser_p.la


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
## libsmtparser.la 用の変数の指定
############################################################

libsmtparser_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libsmtparser_la_SOURCES = $(COMMON_SOURCES)

libsmtparser_la_LIBADD =

libsmtparser_la_LDFLAGS =


############################################################
## libsmtparser_d.la 用の変数の指定
############################################################

libsmtparser_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libsmtparser_d_la_SOURCES = $(COMMON_SOURCES)

libsmtparser_d_la_LIBADD =

libsmtparser_d_la_LDFLAGS =


############################################################
## libsmtparser_p.la 用の変数の指定
############################################################

libsmtparser_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libsmtparser_p_la_SOURCES = $(COMMON_SOURCES)

libsmtparser_p_la_LIBADD =

libsmtparser_p_la_LDFLAGS =
