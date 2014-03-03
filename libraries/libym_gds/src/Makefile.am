## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


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

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

lib_LTLIBRARIES = \
	libym_gds.la \
	libym_gds_d.la \
	libym_gds_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include

AM_YFLAGS = -d -v


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libym_gds.la 用の変数の指定
############################################################

libym_gds_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_gds_la_SOURCES = $(COMMON_SOURCES)

libym_gds_la_LIBADD =

libym_gds_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils.la


############################################################
## libym_gds_d.la 用の変数の指定
############################################################

libym_gds_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_gds_d_la_SOURCES = $(COMMON_SOURCES)

libym_gds_d_la_LIBADD =

libym_gds_d_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils_d.la


############################################################
## libym_gds_p.la 用の変数の指定
############################################################

libym_gds_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_gds_p_la_SOURCES = $(COMMON_SOURCES)

libym_gds_p_la_LIBADD =

libym_gds_p_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils_p.la
