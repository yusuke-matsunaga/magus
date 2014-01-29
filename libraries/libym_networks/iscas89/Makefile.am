## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	iscas89_grammer.cc \
	iscas89_grammer.hh \
	y.output \
	iscas89_grammer.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_iscas89.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

AM_YFLAGS = -d -v


############################################################
## libym_iscas89.la 用の変数の定義
############################################################

libym_iscas89_la_SOURCES = \
	iscas89_nsdef.h \
	iscas89_grammer.yy \
	Iscas89Scanner.h \
	Iscas89Scanner.cc \
	IdHash.h \
	IdHash.cc \
	Iscas89Parser.h \
	Iscas89Parser.cc \
	Iscas89ParserImpl.h \
	Iscas89ParserImpl.cc \
	Iscas89Handler.h \
	Iscas89Handler.cc \
	Iscas89Network.h \
	Iscas89Network.cc \
	Iscas89NetworkHandler.h \
	Iscas89NetworkHandler.cc \
	Iscas89NetworkReader.h \
	Iscas89NetworkReader.cc

libym_iscas89_la_LIBADD =

libym_iscas89_la_LDFLAGS =
