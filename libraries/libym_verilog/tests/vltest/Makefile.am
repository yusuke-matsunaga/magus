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

bin_PROGRAMS = \
	vltest


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_srcdir)/src/parser/include \
	-I$(top_srcdir)/src/parser/scanner \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_builddir)/src/parser/main \
	@POPT_INCLUDES@


#############################################################
# vltest 用の変数の定義
#############################################################

vltest_SOURCES = \
	VlDumper.h \
	VlDumper.cc \
	VlDumperImpl.h \
	VlDumperImpl.cc \
	VlDumperImpl_scope.cc \
	VlDumperImpl_module.cc \
	VlDumperImpl_decl.cc \
	VlDumperImpl_item.cc \
	VlDumperImpl_stmt.cc \
	VlDumperImpl_expr.cc \
	VlTestLineWatcher.h \
	VlTestLineWatcher.cc \
	vltest_rawlex.cc \
	vltest_lex.cc \
	vltest_parse.cc \
	vltest_elaborate.cc \
	vltest_main.cc

vltest_LDADD = \
	$(top_builddir)/src/libym_verilog.la

vltest_LDFLAGS = \
	@POPT_LIBS@
