## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_gbm \
	pylib \
	. \
	tests


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_gbm.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_gbm.la 用の変数の指定
############################################################

libym_gbm_la_SOURCES = \
	Gbm.cc \
	GbmNodeImpl.h \
	GbmNodeImpl.cc \
	GbmInput.h \
	GbmInput.cc \
	GbmAnd.h \
	GbmAnd.cc \
	GbmLut.h \
	GbmLut.cc

libym_gbm_la_LIBADD =

libym_gbm_la_LDFLAGS =
