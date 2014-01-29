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

noinst_LTLIBRARIES = libatpg_dtpg_old.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old/include


############################################################
## libatpg_dtpg_old.la 用の変数の定義
############################################################

libatpg_dtpg_old_la_SOURCES = \
	dtpg_nsdef.h \
	DtpgNode.h \
	DtpgNode.cc \
	DtpgFault.h \
	DtpgFFR.h \
	DtpgNetwork.h \
	DtpgNetwork.cc \
	DtpgSat.h \
	DtpgSat.cc \
	InputLiteral.h \
	InputLiteral.cc

libatpg_dtpg_old_la_LIBADD =

libatpg_dtpg_old_la_LDFLAGS =
