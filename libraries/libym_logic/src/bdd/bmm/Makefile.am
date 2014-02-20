## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIR =


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

noinst_LTLIBRARIES = libym_bdd_bmm.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/bdd/include \
	$(GMP_INCLUDES)


############################################################
## libym_bdd_bmm.la 用の変数の指定
############################################################

libym_bdd_bmm_la_SOURCES = \
	BddMgrModern.h \
	BddMgrModern.cc \
	BmmVar.h \
	BmmVar.cc \
	BmmCompTbl.h \
	BmmCompTbl.cc \
	bmm_ite.cc \
	bmm_isop.cc \
	bmm_onepath.cc \
	bmm_vs.cc \
	bmm_ls.cc

libym_bdd_bmm_la_LIBADD =

libym_bdd_bmm_la_LDFLAGS = \
	$(GMP_LIBS)
