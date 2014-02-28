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
	libmsg.la \
	libmsg_d.la \
	libmsg_p.la


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
	MsgHandler.cc \
	MsgMgr.cc \
	MsgMgrImpl.h \
	MsgMgrImpl.cc


############################################################
## libmsg.la 用の変数の定義
############################################################

libmsg_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libmsg_la_SOURCES = \
	$(COMMON_SOURCES)

libmsg_la_LIBADD =

libmsg_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libmsg_d.la 用の変数の定義
############################################################

libmsg_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libmsg_d_la_SOURCES = \
	$(COMMON_SOURCES)

libmsg_d_la_LIBADD =

libmsg_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libmsg_p.la 用の変数の定義
############################################################

libmsg_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libmsg_p_la_SOURCES = \
	$(COMMON_SOURCES)

libmsg_p_la_LIBADD =

libmsg_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
