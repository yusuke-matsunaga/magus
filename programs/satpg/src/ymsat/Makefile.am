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
	libatpg_ymsat.la \
	libatpg_ymsat_p.la \
	libatpg_ymsat_d.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	AssignList.h \
	AssignList.cc \
	SaBase.h \
	SaBase.cc \
	SaFactory.cc \
	SaUIP1.h \
	SaUIP1.cc \
	SaUIP2.h \
	SaUIP2.cc \
	SatAnalyzer.h \
	SatClause.h \
	SatClause.cc \
	VarHeap.h \
	VarHeap.cc \
	YmSat.h \
	YmSat.cc \
	YmSatR.h \
	YmSatR.cc


############################################################
## libatpg_ymsat.la 用の変数の指定
############################################################

libatpg_ymsat_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libatpg_ymsat_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_ymsat_la_LIBADD =

libatpg_ymsat_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_ymsat_d.la 用の変数の指定
############################################################

libatpg_ymsat_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libatpg_ymsat_d_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_ymsat_d_la_LIBADD =

libatpg_ymsat_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_ymsat_p.la 用の変数の指定
############################################################

libatpg_ymsat_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libatpg_ymsat_p_la_SOURCES = \
	$(COMMON_SOURCES)

libatpg_ymsat_p_la_LIBADD =

libatpg_ymsat_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
