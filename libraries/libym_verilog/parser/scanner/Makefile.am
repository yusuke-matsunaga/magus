## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libym_verilog/parser/include \
	-I$(top_builddir)/libraries/libym_verilog/parser/main

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_parser_scanner.la


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

libym_vl_parser_scanner_la_LIBADD = \
	$(top_builddir)/libraries/libym_utils/libym_utils.la

libym_vl_parser_scanner_la_LDFLAGS =
