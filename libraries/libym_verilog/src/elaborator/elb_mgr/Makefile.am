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
	libvlelab_mgr.la \
	libvlelab_mgr_d.la \
	libvlelab_mgr_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/elaborator/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	AttrHash.cc \
	ElbContAssign.cc \
	ElbDecl.cc \
	ElbExpr.cc \
	ElbMgr.cc \
	ElbParamAssign.cc \
	ElbParameter.cc \
	ElbPrimitive.cc \
	ElbProcess.cc \
	ElbScope.cc \
	ElbStmt.cc \
	ElbUserSystf.cc \
	ModuleHash.cc \
	ObjDict.cc \
	TagDict.cc \
	VlNamedObj.cc


############################################################
## libvlelab_mgr.la
############################################################

libvlelab_mgr_la_CXXFLAGS = -O2

libvlelab_mgr_la_SOURCES = $(COMMON_SOURCES)

libvlelab_mgr_la_LIBADD =

libvlelab_mgr_la_LDFLAGS =


############################################################
## libvlelab_mgr_d.la
############################################################

libvlelab_mgr_d_la_CXXFLAGS = -O2 -g

libvlelab_mgr_d_la_SOURCES = $(COMMON_SOURCES)

libvlelab_mgr_d_la_LIBADD =

libvlelab_mgr_d_la_LDFLAGS =


############################################################
## libvlelab_mgr_p.la
############################################################

libvlelab_mgr_p_la_CXXFLAGS = -O2 -pg

libvlelab_mgr_p_la_SOURCES = $(COMMON_SOURCES)

libvlelab_mgr_p_la_LIBADD =

libvlelab_mgr_p_la_LDFLAGS =
