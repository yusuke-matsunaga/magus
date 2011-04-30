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
	ptview

ptview_SOURCES = \
	VerilogView.h \
	VerilogView.cc \
	ParseTreeModel.h \
	ParseTreeModel.cc \
	ParseTreeView.h \
	ParseTreeView.cc \
	PtNode.h \
	PtNodeImpl.h \
	PtRootNode.h \
	PtNode.cc \
	ptview_main.cc

nodist_ptview_SOURCES = \
	VerilogView.moc.cc \
	ParseTreeView.moc.cc

ptview_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)

ptview_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)

ptview_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)

ptview_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_dotlib/libym_dotlib.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(QT_LIBS) \
	$(LIDADD)

BUILT_SOURCES = \
	VerilogView.moc.cc \
	ParseTreeView.moc.cc

