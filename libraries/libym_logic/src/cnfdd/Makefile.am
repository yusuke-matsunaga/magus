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
	libym_logic_cnfdd.la \
	libym_logic_cnfdd_d.la \
	libym_logic_cnfdd_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	$(GMP_INCLUDES)


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CompTbl.h \
	CompTbl.cc \
	CNFddEdge.h \
	CNFddNode.h \
	CNFddVar.h \
	CNFdd.cc \
	CNFddMgr.cc \
	CNFddMgrImpl.h \
	CNFddMgrImpl.cc \
	Op.h \
	UniOp.h \
	UniOp.cc \
	UniNOp.h \
	UniNOp.cc \
	UniVOp.h \
	UniVOp.cc \
	BinOp.h \
	BinOp.cc \
	ConOp.h \
	ConOp.cc \
	DisOp.h \
	DisOp.cc \
	DiffOp.h \
	DiffOp.cc \
	DiffOp2.h \
	DiffOp2.cc \
	MergeOp.h \
	MergeOp.cc \
	MinOp.h \
	MinOp.cc \
	CutOp.h \
	CutOp.cc \
	LitPOp.h \
	LitPOp.cc \
	LitNOp.h \
	LitNOp.cc \
	Cof0Op.h \
	Cof0Op.cc \
	CofPOp.h \
	CofPOp.cc \
	CofNOp.h \
	CofNOp.cc \
	SupOp.h \
	SupOp.cc \
	count.cc \
	IdMgr.h \
	IdMgr.cc \
	Printer.h \
	Printer.cc \
	Printer2.h \
	Printer2.cc \
	Dumper.h \
	Dumper.cc \
	Restorer.h \
	Restorer.cc


############################################################
## libym_logic_cnfdd.la 用の変数の指定
############################################################

libym_logic_cnfdd_la_CXXFLAGS = -O2

libym_logic_cnfdd_la_SOURCES = $(COMMON_SOURCES)

libym_logic_cnfdd_la_LIBADD =

libym_logic_cnfdd_la_LDFLAGS =


############################################################
## libym_logic_cnfdd_d.la 用の変数の指定
############################################################

libym_logic_cnfdd_d_la_CXXFLAGS = -O2 -g

libym_logic_cnfdd_d_la_SOURCES = $(COMMON_SOURCES)

libym_logic_cnfdd_d_la_LIBADD =

libym_logic_cnfdd_d_la_LDFLAGS =


############################################################
## libym_logic_cnfdd_p.la 用の変数の指定
############################################################

libym_logic_cnfdd_p_la_CXXFLAGS = -O2 -pg

libym_logic_cnfdd_p_la_SOURCES = $(COMMON_SOURCES)

libym_logic_cnfdd_p_la_LIBADD =

libym_logic_cnfdd_p_la_LDFLAGS =
