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
	libym_vl_elab_main.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/elaborator/include


############################################################
## libym_vl_elab_main.la 用の変数の定義
############################################################

libym_vl_elab_main_la_SOURCES = \
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

libym_vl_elab_main_la_LIBADD =

libym_vl_elab_main_la_LDFLAGS =
