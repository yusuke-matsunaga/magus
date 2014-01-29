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
	libym_vl_elab_elb_mgr.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/elaborator/include


############################################################
## libym_vl_elab_elb_mgr.la
############################################################

libym_vl_elab_elb_mgr_la_SOURCES = \
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

libym_vl_elab_elb_mgr_la_LIBADD =

libym_vl_elab_elb_mgr_la_LDFLAGS =
