## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ci \
	mislib \
	dotlib \
	libcomp


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

lib_LTLIBRARIES = \
	libym_cell.la \
	libym_cell_d.la \
	libym_cell_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CellFFInfo.cc \
	CellLatchInfo.cc \
	display.cc


############################################################
## libym_cell.la 用の変数の指定
############################################################

libym_cell_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libym_cell_la_SOURCES = \
	$(COMMON_SOURCES)

libym_cell_la_LIBADD = \
	ci/libym_cell_ci.la \
	dotlib/libym_cell_dotlib.la \
	mislib/libym_cell_mislib.la \
	libcomp/libym_cell_libcomp.la

libym_cell_la_LDFLAGS = \
	$(top_builddir)/../libym_logic/src/libym_logic.la \
	$(AM_LDFLAGS)



############################################################
## libym_cell_d.la 用の変数の指定
############################################################

libym_cell_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libym_cell_d_la_SOURCES = \
	$(COMMON_SOURCES)

libym_cell_d_la_LIBADD = \
	ci/libym_cell_ci_d.la \
	dotlib/libym_cell_dotlib_d.la \
	mislib/libym_cell_mislib_d.la \
	libcomp/libym_cell_libcomp_d.la

libym_cell_d_la_LDFLAGS = \
	$(top_builddir)/../libym_logic/src/libym_logic_d.la \
	$(AM_LDFLAGS)


############################################################
## libym_cell_p.la 用の変数の指定
############################################################

libym_cell_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libym_cell_p_la_SOURCES = \
	$(COMMON_SOURCES)

libym_cell_p_la_LIBADD = \
	ci/libym_cell_ci.la \
	dotlib/libym_cell_dotlib_p.la \
	mislib/libym_cell_mislib_p.la \
	libcomp/libym_cell_libcomp_p.la

libym_cell_p_la_LDFLAGS = \
	$(top_builddir)/../libym_logic/src/libym_logic_p.la \
	$(AM_LDFLAGS)
