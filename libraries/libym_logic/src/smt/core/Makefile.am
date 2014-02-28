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
	libsmtcore.la \
	libsmtcore_d.la \
	libsmtcore_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	SmtFunImpl.h \
	SmtFunImpl.cc \
	SmtFunValidatorImpl.h \
	SmtFunValidatorImpl.cc \
	SmtSolverImpl.h \
	SmtSolverImpl.cc \
	SmtSortImpl.h \
	SmtSortImpl.cc \
	SmtTermImpl.h \
	SmtTermImpl.cc \
	SmtVarImpl.h \
	SmtVarImpl.cc


############################################################
## libsmtcore.la 用の変数の指定
############################################################

libsmtcore_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libsmtcore_la_SOURCES = \
	$(COMMON_SOURCES)

libsmtcore_la_LIBADD =

libsmtcore_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libsmtcore_d.la 用の変数の指定
############################################################

libsmtcore_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libsmtcore_d_la_SOURCES = \
	$(COMMON_SOURCES)

libsmtcore_d_la_LIBADD =

libsmtcore_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libsmtcore_p.la 用の変数の指定
############################################################

libsmtcore_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libsmtcore_p_la_SOURCES = \
	$(COMMON_SOURCES)

libsmtcore_p_la_LIBADD =

libsmtcore_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
