## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

include $(top_srcdir)/etc/autotroll.mk

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libym_verilog \
	-I$(top_builddir)/libym_verilog \
	-I$(top_srcdir)/libym_verilog/parser/include \
	-I$(top_builddir)/libym_verilog/parser/main \
	-I$(top_srcdir)/libym_verilog/parser/scanner

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	vlview

vlview_SOURCES = \
	VlView.h \
	VlView.cc \
	VlView.moc.cc \
	VlTokenList.h \
	VlTokenList.cc \
	VlTokenView.h \
	VlTokenView.cc \
	VlTokenView.moc.cc \
	VlParseTreeModel.h \
	VlParseTreeModel.cc \
	VlParseTreeView.h \
	VlParseTreeView.cc \
	VlParseTreeView.moc.cc \
	VlPtNode.h \
	VlPtNode.cc \
	VlPtNode_root.h \
	VlPtNode_root.cc \
	VlPtNode_udp.h \
	VlPtNode_udp.cc \
	VlPtNode_module.h \
	VlPtNode_module.cc \
	VlPtNode_port.h \
	VlPtNode_port.cc \
	VlPtNode_decl.h \
	VlPtNode_decl.cc \
	VlPtNode_item.h \
	VlPtNode_item.cc \
	VlPtNode_stmt.h \
	VlPtNode_stmt.cc \
	VlPtNode_expr.h \
	VlPtNode_expr.cc \
	VlPtNode_misc.h \
	VlPtNode_misc.cc \
	vlview_main.cc

vlview_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)

vlview_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)

vlview_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)

vlview_LDADD = \
	$(top_builddir)/libym_verilog/libym_verilog.la \
	$(top_builddir)/libym_utils/libym_utils.la \
	$(QT_LIBS) \
	$(LIDADD)

BUILT_SOURCES = \
	VlView.moc.cc \
	VlTokenView.moc.cc \
	VlParseTreeView.moc.cc