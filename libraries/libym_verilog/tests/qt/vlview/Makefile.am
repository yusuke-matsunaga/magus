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
	vlview


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CXXFLAGS = \
	$(QT_CXXFLAGS)

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include \
	$(QT_CPPFLAGS)


############################################################
## vlview 用の変数の定義
############################################################

vlview_SOURCES = \
	VlView.h \
	VlView.cc \
	VerilogModel.h \
	VerilogModel.cc \
	VerilogView.h \
	VerilogView.cc \
	VmNode.h \
	VmNode.cc \
	VmRootNode.h \
	VmRootNode.cc \
	VmUdpNode.h \
	VmUdpNode.cc \
	VmModuleNode.h \
	VmModuleNode.cc \
	VmDeclNode.h \
	VmDeclNode.cc \
	VmItemNode.h \
	VmItemNode.cc \
	VmStmtNode.h \
	VmStmtNode.cc \
	VmExprNode.h \
	VmExprNode.cc \
	VmMiscNode.h \
	VmMiscNode.cc \
	vlview_main.cc

nodist_vlview_SOURCES = \
	VlView.moc.cc \
	VerilogView.moc.cc

BUILT_SOURCES = \
	VlView.moc.cc \
	VerilogView.moc.cc

vlview_LDADD = \
	$(top_builddir)/src/libym_verilog.la \
	$(QT_LIBS) \
	$(LDADD)

vlview_LDFLAGS  = \
	$(QT_LDFLAGS) \
	$(LDFLAGS)
