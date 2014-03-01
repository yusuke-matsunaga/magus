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
	libym_imp.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	@GMP_INCLUDES@


############################################################
## libym_imp.la 用の変数の定義
############################################################

libym_imp_la_SOURCES = \
	BNodeMap.cc \
	ImpInfo.cc \
	ImpHash.cc \
	ImpCell.h \
	ImpBase.cc \
	ImpMgr.cc \
	ImpNode.cc \
	ImpInput.h \
	ImpInput.cc \
	ImpAnd.h \
	ImpAnd.cc \
	ImpListRec.h \
	ImpListRec.cc \
	ImpListRec2.h \
	ImpListRec2.cc \
	ImpListRec3.h \
	ImpListRec3.cc \
	StrImp.h \
	StrImp.cc \
	ContraImp.h \
	ContraImp.cc \
	CnfImp.h \
	CnfImp.cc \
	CnfClause.h \
	CnfImp2.h \
	CnfImp2.cc \
	SatImp.h \
	SatImp.cc \
	ConstImp.h \
	ConstImp.cc \
	RlImp.h \
	RlImp.cc \
	NaImp.h \
	NaImp.cc \
	ImpValList.h \
	ImpValList.cc

libym_imp_la_LIBADD =

libym_imp_la_LDFLAGS =
