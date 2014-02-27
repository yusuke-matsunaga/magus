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

noinst_LTLIBRARIES = \
	libiscas89.la \
	libiscas89_d.la \
	libiscas89_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include

AM_YFLAGS = -d -v


############################################################
## libiscas89.la 用の変数の定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libiscas89.la 用の変数の定義
############################################################

libiscas89_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libiscas89_la_SOURCES = $(COMMON_SOURCES)

libiscas89_la_LIBADD =

libiscas89_la_LDFLAGS =


############################################################
## libiscas89_d.la 用の変数の定義
############################################################

libiscas89_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libiscas89_d_la_SOURCES = $(COMMON_SOURCES)

libiscas89_d_la_LIBADD =

libiscas89_d_la_LDFLAGS =


############################################################
## libiscas89_p.la 用の変数の定義
############################################################

libiscas89_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libiscas89_p_la_SOURCES = $(COMMON_SOURCES)

libiscas89_p_la_LIBADD =

libiscas89_p_la_LDFLAGS =
