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

noinst_LTLIBRARIES = libym_bdd_base.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/bdd/include \
	$(GMP_INCLUDES)


############################################################
## libym_bdd_base.la 用の変数の指定
############################################################

libym_bdd_base_la_SOURCES = \
	Bdd.cc \
	BddMgr.cc \
	BddMgrImpl.cc \
	BddLitSet.cc \
	BddVarSet.cc \
	BddVector.cc \
	BddList.cc \
	CompTbl.h \
	CompTbl.cc \
	CompTbl1.h \
	CompTbl1.cc \
	CompTbl2.h \
	CompTbl2.cc \
	CompTbl2e.h \
	CompTbl2e.cc \
	CompTbl3.h \
	CompTbl3.cc \
	BddOp.h \
	BddUniOp.h \
	BddUniOp.cc \
	CofOp.h \
	CofOp.cc \
	XcOp.h \
	XcOp.cc \
	BddBinOp.h \
	BddBinOp.cc \
	AndOp.h \
	AndOp.cc \
	XorOp.h \
	XorOp.cc \
	IntsecOp.h \
	IntsecOp.cc \
	GcOp.h \
	GcOp.cc \
	BddTriOp.h \
	BddTriOp.cc \
	IteOp.h \
	IteOp.cc \
	SymOp.h \
	SymOp.cc \
	CountOp.h \
	CountOp.cc \
	McOp.h \
	McOp.cc \
	W0Op.h \
	W0Op.cc \
	W1Op.h \
	W1Op.cc \
	SupOp.h \
	SupOp.cc \
	SmoothOp.h \
	SmoothOp.cc \
	AeOp.h \
	AeOp.cc \
	IdMgr.h \
	IdMgr.cc \
	Printer.h \
	Printer.cc \
	Dumper.h \
	Dumper.cc \
	Restorer.h \
	Restorer.cc

libym_bdd_base_la_LIBADD =

libym_bdd_base_la_LDFLAGS =
