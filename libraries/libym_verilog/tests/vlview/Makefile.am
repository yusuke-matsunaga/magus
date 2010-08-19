## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

include $(top_srcdir)/../../etc/autotroll.mk

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	vlview

vlview_SOURCES = \
	VlView.h \
	VlView.cc \
	VerilogModel.h \
	VerilogModel.cc \
	VerilogView.h \
	VerilogView.cc \
	VmNode.h \
	VmNode.cc \
	VmNode_root.h \
	VmNode_root.cc \
	VmNode_udp.h \
	VmNode_udp.cc \
	VmNode_module.h \
	VmNode_module.cc \
	VmNode_port.h \
	VmNode_port.cc \
	VmNode_decl.h \
	VmNode_decl.cc \
	VmNode_item.h \
	VmNode_item.cc \
	VmNode_stmt.h \
	VmNode_stmt.cc \
	VmNode_expr.h \
	VmNode_expr.cc \
	VmNode_misc.h \
	VmNode_misc.cc \
	vlview_main.cc

nodist_vlview_SOURCES = \
	VlView.moc.cc \
	VerilogView.moc.cc

vlview_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)

vlview_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)

vlview_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)

vlview_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(QT_LIBS) \
	$(LIDADD)

BUILT_SOURCES = \
	VlView.moc.cc \
	VerilogView.moc.cc

