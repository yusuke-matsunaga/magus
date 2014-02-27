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
	libvlparser_scanner.la \
	libvlparser_scanner_d.la \
	libvlparser_scanner_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/parser/include \
	-I$(top_builddir)/src/parser/main


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	parser_common.h \
	print_token.h \
	RsrvWordDic.h \
	RsrvWordDic.cc \
	TokenInfo.h \
	TokenInfo.cc \
	TokenList.h \
	TokenList.cc \
	InputFile.h \
	InputFile.cc \
	InputMgr.h \
	InputMgr.cc \
	MacroSource.h \
	MacroSource.cc \
	RawLex.cc \
	Lex.cc \
	LexState.cc \
	LexPlugin.cc \
	LexPluginDict.cc \
	StdLexPlugin.h \
	StdLexPlugin.cc \
	LexCondState.h \
	LexCondPlugin.h \
	LexCondPlugin.cc \
	LexMacroPlugin.h \
	LexMacroPlugin.cc \
	LexModuleState.cc \
	LexModulePlugin.h \
	LexModulePlugin.cc


############################################################
## libvlparser_scanner.la
############################################################

libvlparser_scanner_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libvlparser_scanner_la_SOURCES = $(COMMON_SOURCES)

libvlparser_scanner_la_LIBADD =

libvlparser_scanner_la_LDFLAGS =


############################################################
## libvlparser_scanner.la
############################################################

libvlparser_scanner_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libvlparser_scanner_d_la_SOURCES = $(COMMON_SOURCES)

libvlparser_scanner_d_la_LIBADD =

libvlparser_scanner_d_la_LDFLAGS =


############################################################
## libvlparser_scanner_p.la
############################################################

libvlparser_scanner_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libvlparser_scanner_p_la_SOURCES = $(COMMON_SOURCES)

libvlparser_scanner_p_la_LIBADD =

libvlparser_scanner_p_la_LDFLAGS =
