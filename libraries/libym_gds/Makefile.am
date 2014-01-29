## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_gds \
	. \
	tests


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	gds_grammer.cc \
	gds_grammer.hh \
	y.output \
	gds_grammer.output

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

noinst_LTLIBRARIES = libym_gds.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

AM_YFLAGS = -d -v


############################################################
## libym_gds.la 用の変数の指定
############################################################

libym_gds_la_SOURCES = \
	gds_grammer.yy \
	Msg.cc \
	GdsDumper.cc \
	GdsParser.cc \
	GdsRecTable.h \
	GdsRecTable.cc \
	GdsRecMgr.cc \
	GdsRecord.cc \
	GdsScanner.cc \
	GdsAref.h \
	GdsAref.cc \
	GdsBoundary.h \
	GdsBoundary.cc \
	GdsBox.h \
	GdsBox.cc \
	GdsData.cc \
	GdsElement.cc \
	GdsFormat.cc \
	GdsNode.h \
	GdsNode.cc \
	GdsRefBase.h \
	GdsRefBase.cc \
	GdsPath.h \
	GdsPath.cc \
	GdsSref.h \
	GdsSref.cc \
	GdsStruct.cc \
	GdsText.h \
	GdsText.cc

libym_gds_la_LIBADD =

libym_gds_la_LDFLAGS =
