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
	libym_vl_parser_pt_mgr.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/parser/cpt \
	-I$(top_srcdir)/parser/spt


############################################################
## libym_vl_parser.la
############################################################

libym_vl_parser_pt_mgr_la_SOURCES = \
	PtiFactory.cc \
	PtMgr.cc \
	PtiBase.cc \
	PtDumper.cc

libym_vl_parser_pt_mgr_la_LIBADD =

libym_vl_parser_pt_mgr_la_LDFLAGS =
