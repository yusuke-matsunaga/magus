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
	libym_vl_parser_scanner.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_builddir)/parser/main


############################################################
## libym_vl_parser_scanner.la
############################################################

libym_vl_parser_scanner_la_SOURCES = \
	parser.h \
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

libym_vl_parser_scanner_la_LIBADD =

libym_vl_parser_scanner_la_LDFLAGS =
