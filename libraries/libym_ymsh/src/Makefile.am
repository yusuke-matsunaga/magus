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

lib_LTLIBRARIES = \
	libym_ymsh.la \
	libym_ymsh_d.la \
	libym_ymsh_p.la


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
	Ymsh.cc \
	YmshImpl.h \
	YmshImpl.cc \
	YmshCmdTable.h \
	YmshCmdTable.cc \
	YmshCmd.cc \
	YmshObjTable.h \
	YmshObjTable.cc \
	SetCmd.h \
	SetCmd.cc \
	AliasCmd.h \
	AliasCmd.cc \
	ExitCmd.h \
	ExitCmd.cc \
	DirCmd.h \
	DirCmd.cc


############################################################
## libym_ymsh.la 用の変数の定義
############################################################

libym_ymsh_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_ymsh_la_SOURCES = $(COMMON_SOURCES)

libym_ymsh_la_LIBADD =

libym_ymsh_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils.la \
	-lreadline -lncurses


############################################################
## libym_ymsh_d.la 用の変数の定義
############################################################

libym_ymsh_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_ymsh_d_la_SOURCES = $(COMMON_SOURCES)

libym_ymsh_d_la_LIBADD =

libym_ymsh_d_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils_d.la \
	-lreadline -lncurses


############################################################
## libym_ymsh_p.la 用の変数の定義
############################################################

libym_ymsh_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_ymsh_p_la_SOURCES = $(COMMON_SOURCES)

libym_ymsh_p_la_LIBADD =

libym_ymsh_p_la_LDFLAGS = \
	$(top_builddir)/../libym_utils/src/libym_utils_p.la \
	-lreadline -lncurses
