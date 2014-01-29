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

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	npngen \
	cannonical \
	genaig


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@


############################################################
## genaig 用の変数の定義
############################################################

genaig_SOURCES = \
	genaig.cc \
	npn3cannon.h \
	npn3perm.h \
	npn3rep.h \
	npn4cannon.h \
	npn4perm.h \
	npn4rep.h \
	AigPat.h \
	FuncXform.h \
	GenAig.h \
	GenAig.cc \
	GenAigNaive.h \
	GenAigNaive.cc \
	GenAigDj.h \
	GenAigDj.cc

genaig_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la

genaig_LDFLAGS = \
	@POPT_LIBS@


############################################################
## npngen 用の変数の定義
############################################################

npngen_SOURCES = \
	npngen.cc

npngen_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

npngen_LDFLAGS =


############################################################
## cannonical 用の変数の定義
############################################################

cannonical_SOURCES = \
	cannonical.cc

cannonical_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

cannonical_LDFLAGS =
