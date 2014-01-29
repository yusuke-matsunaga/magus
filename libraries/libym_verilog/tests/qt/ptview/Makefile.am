## Process this file with automake to produce Makefile.in

############################################################
## autotroll のおまじない
############################################################

include $(top_srcdir)/../../etc/autotroll.mk


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
	ptview


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CXXFLAGS = \
	$(QT_CXXFLAGS)

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/parser/scanner \
	-I$(top_builddir)/parser/main \
	$(QT_CPPFLAGS)


############################################################
## ptview 用の変数の定義
############################################################

ptview_SOURCES = \
	VerilogView.h \
	VerilogView.cc \
	VerilogSyntaxHighlighter.h \
	VerilogSyntaxHighlighter.cc \
	TokenListModel.h \
	TokenListModel.cc \
	TokenView.h \
	TokenView.cc \
	ParseTreeModel.h \
	ParseTreeModel.cc \
	ParseTreeView.h \
	ParseTreeView.cc \
	PtNode.h \
	PtNode.cc \
	PtNode_root.h \
	PtNode_root.cc \
	PtNode_udp.h \
	PtNode_udp.cc \
	PtNode_module.h \
	PtNode_module.cc \
	PtNode_port.h \
	PtNode_port.cc \
	PtNode_decl.h \
	PtNode_decl.cc \
	PtNode_item.h \
	PtNode_item.cc \
	PtNode_stmt.h \
	PtNode_stmt.cc \
	PtNode_expr.h \
	PtNode_expr.cc \
	PtNode_misc.h \
	PtNode_misc.cc \
	GoToLine.h \
	GoToLine.cc \
	ptview_main.cc

nodist_ptview_SOURCES = \
	VerilogView.moc.cc \
	TokenView.moc.cc \
	ParseTreeView.moc.cc \
	GoToLine.moc.cc

BUILT_SOURCES = \
	VerilogView.moc.cc \
	TokenView.moc.cc \
	ParseTreeView.moc.cc \
	GoToLine.moc.cc

ptview_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(QT_LIBS) \
	$(LDADD)

ptview_LDFLAGS  = \
	$(QT_LDFLAGS) \
	$(LDFLAGS)
