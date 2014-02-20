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

noinst_LTLIBRARIES = libym_logic_aig.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## libym_aig.la 用の変数の指定
############################################################

libym_logic_aig_la_SOURCES = \
	AigNode.h \
	AigNode.cc \
	AigMgr.cc \
	AigMgrImpl.h \
	AigMgrImpl.cc \
	AigSatMgr.cc

#	AigSatData.h \
#	AigSatMgr.cc \
#	AigTemplate.h \
#	AigTemplate.cc

libym_logic_aig_la_LIBADD =

libym_logic_aig_la_LDFLAGS =
