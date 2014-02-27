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
	libvlparser_pt_mgr.la \
	libvlparser_pt_mgr_d.la \
	libvlparser_pt_mgr_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/parser/include \
	-I$(top_srcdir)/src/parser/cpt \
	-I$(top_srcdir)/src/parser/spt


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	PtiFactory.cc \
	PtMgr.cc \
	PtiBase.cc \
	PtDumper.cc


############################################################
## libym_vl_parser.la
############################################################

libvlparser_pt_mgr_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libvlparser_pt_mgr_la_SOURCES = $(COMMON_SOURCES)

libvlparser_pt_mgr_la_LIBADD =

libvlparser_pt_mgr_la_LDFLAGS =


############################################################
## libym_vl_parser_d.la
############################################################

libvlparser_pt_mgr_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libvlparser_pt_mgr_d_la_SOURCES = $(COMMON_SOURCES)

libvlparser_pt_mgr_d_la_LIBADD =

libvlparser_pt_mgr_d_la_LDFLAGS =


############################################################
## libym_vl_parser_p.la
############################################################

libvlparser_pt_mgr_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libvlparser_pt_mgr_p_la_SOURCES = $(COMMON_SOURCES)

libvlparser_pt_mgr_p_la_LIBADD =

libvlparser_pt_mgr_p_la_LDFLAGS =
