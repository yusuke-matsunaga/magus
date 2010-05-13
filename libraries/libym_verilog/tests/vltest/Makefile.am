## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libym_verilog \
	-I$(top_builddir)/libym_verilog \
	-I$(top_srcdir)/libym_verilog/parser/include \
	-I$(top_builddir)/libym_verilog/parser/main \
	-I$(top_srcdir)/libym_verilog/parser/scanner \
	@POPT_INCLUDES@

YFLAGS=-d -v

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	vltest


#############################################################
# vltest
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
	$(top_builddir)/libym_verilog/libym_verilog.la \
	$(top_builddir)/libym_utils/libym_utils.la \
	@POPT_LIBS@
