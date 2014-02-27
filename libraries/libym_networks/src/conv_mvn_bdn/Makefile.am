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
	libconv_mvn_bdn.la \
	libconv_mvn_bdn_d.la \
	libconv_mvn_bdn_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	MvnBdnMap.cc \
	MvnBdnConv.cc \
	MvnConv.h \
	ThroughConv.h \
	ThroughConv.cc \
	NotConv.h \
	NotConv.cc \
	AndConv.h \
	AndConv.cc \
	OrConv.h \
	OrConv.cc \
	XorConv.h \
	XorConv.cc \
	RandConv.h \
	RandConv.cc \
	RorConv.h \
	RorConv.cc \
	RxorConv.h \
	RxorConv.cc \
	CmplConv.h \
	CmplConv.cc \
	AddConv.h \
	AddConv.cc \
	SubConv.h \
	SubConv.cc \
	MultConv.h \
	MultConv.cc \
	DivConv.h \
	DivConv.cc \
	ModConv.h \
	ModConv.cc \
	PowConv.h \
	PowConv.cc \
	SllConv.h \
	SllConv.cc \
	SrlConv.h \
	SrlConv.cc \
	SlaConv.h \
	SlaConv.cc \
	SraConv.h \
	SraConv.cc \
	EqConv.h \
	EqConv.cc \
	LtConv.h \
	LtConv.cc \
	CaseEqConv.h \
	CaseEqConv.cc \
	IteConv.h \
	IteConv.cc \
	ConcatConv.h \
	ConcatConv.cc \
	ConstBitSelectConv.h \
	ConstBitSelectConv.cc \
	ConstPartSelectConv.h \
	ConstPartSelectConv.cc


############################################################
## libconv_mvn_bdn.la 用の変数の定義
############################################################

libconv_mvn_bdn_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libconv_mvn_bdn_la_SOURCES = $(COMMON_SOURCES)

libconv_mvn_bdn_la_LIBADD =

libconv_mvn_bdn_la_LDFLAGS =


############################################################
## libconv_mvn_bdn_d.la 用の変数の定義
############################################################

libconv_mvn_bdn_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libconv_mvn_bdn_d_la_SOURCES = $(COMMON_SOURCES)

libconv_mvn_bdn_d_la_LIBADD =

libconv_mvn_bdn_d_la_LDFLAGS =


############################################################
## libconv_mvn_bdn_p.la 用の変数の定義
############################################################

libconv_mvn_bdn_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libconv_mvn_bdn_p_la_SOURCES = $(COMMON_SOURCES)

libconv_mvn_bdn_p_la_LIBADD =

libconv_mvn_bdn_p_la_LDFLAGS =
