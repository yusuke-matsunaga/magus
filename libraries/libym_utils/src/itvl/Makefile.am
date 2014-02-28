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
	libitvl.la \
	libitvl_d.la \
	libitvl_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	ItvlCell.h \
	ItvlMgrImpl.h \
	ItvlMgrImpl.cc \
	ItvlMgr.cc \
	NameMgr.cc


############################################################
## libitvl.la 用の変数の定義
############################################################

libitvl_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libitvl_la_SOURCES = \
	$(COMMON_SOURCES)

libitvl_la_LIBADD =

libitvl_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libitvl_d.la 用の変数の定義
############################################################

libitvl_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libitvl_d_la_SOURCES = \
	$(COMMON_SOURCES)

libitvl_d_la_LIBADD =

libitvl_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libitvl_p.la 用の変数の定義
############################################################

libitvl_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libitvl_p_la_SOURCES = \
	$(COMMON_SOURCES)

libitvl_p_la_LIBADD =

libitvl_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
