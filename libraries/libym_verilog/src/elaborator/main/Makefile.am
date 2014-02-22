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
	libvlelab_main.la \
	libvlelab_main_d.la \
	libvlelab_main_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/parser/include \
	-I$(top_srcdir)/src/elaborator/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CfDict.cc \
	AttrDict.cc \
	ElbEnv.h \
	ElbEnv.cc \
	ElbProxy.h \
	ElbProxy.cc \
	ElbParamCon.h \
	UdpGen.h \
	UdpGen.cc \
	ModuleGen.h \
	ModuleGen.cc \
	DeclGen.h \
	DeclGen.cc \
	ItemGen.h \
	ItemGen_main.cc \
	ItemGen_taskfunc.cc \
	ItemGen_module_inst.cc \
	ItemGen_primitive.cc \
	StmtGen.h \
	StmtGen_main.cc \
	StmtGen_assign.cc \
	StmtGen_block.cc \
	StmtGen_cond.cc \
	ExprGen.h \
	ExprGen_main.cc \
	ExprGen_operation.cc \
	ExprGen_primary.cc \
	ExprGen_funccall.cc \
	ExprGen_error.cc \
	AttrGen.h \
	AttrGen.cc \
	Elaborator.cc


############################################################
## libvlelab_main.la 用の変数の定義
############################################################

libvlelab_main_la_CXXFLAGS = -O2

libvlelab_main_la_SOURCES = $(COMMON_SOURCES)

libvlelab_main_la_LIBADD =

libvlelab_main_la_LDFLAGS =


############################################################
## libvlelab_main.la 用の変数の定義
############################################################

libvlelab_main_d_la_CXXFLAGS = -O2 -g

libvlelab_main_d_la_SOURCES = $(COMMON_SOURCES)

libvlelab_main_d_la_LIBADD =

libvlelab_main_d_la_LDFLAGS =


############################################################
## libvlelab_main_p.la 用の変数の定義
############################################################

libvlelab_main_p_la_CXXFLAGS = -O2 -pg

libvlelab_main_p_la_SOURCES = $(COMMON_SOURCES)

libvlelab_main_p_la_LIBADD =

libvlelab_main_p_la_LDFLAGS =
