## Process this file with automake to produce Makefile.in


############################################################
## autotroll のおまじない
############################################################

include $(top_srcdir)/../../etc/autotroll.mk


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
	ptview


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CXXFLAGS = \
	$(QT_CXXFLAGS)

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(QT_CPPFLAGS)


############################################################
## ptview 用の変数の指定
############################################################

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

ptview_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)

ptview_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(QT_LIBS) \
	$(LIDADD)

BUILT_SOURCES = \
	VerilogView.moc.cc \
	ParseTreeView.moc.cc
