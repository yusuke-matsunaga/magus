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
	libzdd.la \
	libzdd_d.la \
	libzdd_p.la


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
	ZddEdge.h \
	ZddNode.h \
	ZddVar.h \
	Zdd.cc \
	ZddVector.cc \
	ZddList.cc \
	ZddMgr.cc \
	ZddMgrImpl.h \
	ZddMgrImpl.cc \
	ZddOp.h \
	ZddUniOp.h \
	ZddUniOp.cc \
	ZddBinOp.h \
	ZddBinOp.cc \
	CapOp.h \
	CapOp.cc \
	CupOp.h \
	CupOp.cc \
	DiffOp.h \
	DiffOp.cc \
	SwapOp.h \
	SwapOp.cc \
	Cof0Op.h \
	Cof0Op.cc \
	Cof1Op.h \
	Cof1Op.cc \
	SupOp.h \
	SupOp.cc \
	MergeOp.h \
	MergeOp.cc \
	MergeOp2.h \
	MergeOp2.cc \
	NeOp.h \
	NeOp.cc \
	MsOp.h \
	MsOp.cc \
	Ms2Op.h \
	Ms2Op.cc \
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
## libzdd.la 用の変数の指定
############################################################

libzdd_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libzdd_la_SOURCES = $(COMMON_SOURCES)

libzdd_la_LIBADD =

libzdd_la_LDFLAGS =


############################################################
## libzdd_d.la 用の変数の指定
############################################################

libzdd_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libzdd_d_la_SOURCES = $(COMMON_SOURCES)

libzdd_d_la_LIBADD =

libzdd_d_la_LDFLAGS =


############################################################
## libzdd_p.la 用の変数の指定
############################################################

libzdd_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libzdd_p_la_SOURCES = $(COMMON_SOURCES)

libzdd_p_la_LIBADD =

libzdd_p_la_LDFLAGS =
