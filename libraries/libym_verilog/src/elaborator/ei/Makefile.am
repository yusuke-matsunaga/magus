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

noinst_LTLIBRARIES = \
	libvlelab_ei.la \
	libvlelab_ei_d.la \
	libvlelab_ei_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/elaborator/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	EiFactory.h \
	EiFactory.cc \
	EiToplevel.h \
	EiToplevel.cc \
	EiUdp.h \
	EiUdp.cc \
	EiModule.h \
	EiModule.cc \
	EiPort.h \
	EiPort.cc \
	EiIODecl.h \
	EiIODecl.cc \
	EiDecl.h \
	EiDecl.cc \
	EiDeclHead.h \
	EiDeclHead.cc \
	EiDeclArray.h \
	EiDeclArray.cc \
	EiParameter.h \
	EiParameter.cc \
	EiImpNet.h \
	EiImpNet.cc \
	EiGenvar.h \
	EiGenvar.cc \
	EiContAssign.h \
	EiContAssign.cc \
	EiProcess.h \
	EiProcess.cc \
	EiParamAssign.h \
	EiParamAssign.cc \
	EiPrimitive.h \
	EiPrimitive.cc \
	EiTaskFunc.h \
	EiTaskFunc.cc \
	EiGenBlock.h \
	EiGenBlock.cc \
	EiStmt.h \
	EiStmt.cc \
	EiAssignment.h \
	EiAssignment.cc \
	EiBlockScope.h \
	EiBlockScope.cc \
	EiBlockStmt.h \
	EiBlockStmt.cc \
	EiCondStmt.h \
	EiCondStmt.cc \
	EiMiscStmt.h \
	EiMiscStmt.cc \
	EiControl.h \
	EiControl.cc \
	EiDelay.h \
	EiDelay.cc \
	EiRange.h \
	EiRange.cc \
	EiLhs.h \
	EiLhs.cc \
	EiExpr.h \
	EiExpr.cc \
	EiOperation.h \
	EiUnaryOp.h \
	EiUnaryOp.cc \
	EiBinaryOp.h \
	EiBinaryOp.cc \
	EiTernaryOp.h \
	EiTernaryOp.cc \
	EiConcatOp.h \
	EiConcatOp.cc \
	EiPrimary.h \
	EiPrimary.cc \
	EiBitSelect.h \
	EiBitSelect.cc \
	EiPartSelect.h \
	EiPartSelect.cc \
	EiFuncCall.h \
	EiFuncCall.cc \
	EiConstant.h \
	EiConstant.cc \
	EiAttribute.h \
	EiAttribute.cc


############################################################
## libvlelab_ei.la 用の変数の定義
############################################################

libvlelab_ei_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libvlelab_ei_la_SOURCES = $(COMMON_SOURCES)

libvlelab_ei_la_LIBADD =

libvlelab_ei_la_LDFLAGS =


############################################################
## libvlelab_ei_d.la 用の変数の定義
############################################################

libvlelab_ei_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libvlelab_ei_d_la_SOURCES = $(COMMON_SOURCES)

libvlelab_ei_d_la_LIBADD =

libvlelab_ei_d_la_LDFLAGS =


############################################################
## libvlelab_ei_p.la 用の変数の定義
############################################################

libvlelab_ei_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libvlelab_ei_p_la_SOURCES = $(COMMON_SOURCES)

libvlelab_ei_p_la_LIBADD =

libvlelab_ei_p_la_LDFLAGS =
