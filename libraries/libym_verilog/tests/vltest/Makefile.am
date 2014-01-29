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
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/parser/scanner \
	-I$(top_builddir)/parser/main \
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
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

vltest_LDFLAGS = \
	@POPT_LIBS@
