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
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	npngen \
	cannonical \
	ident \
	read_table \
	xform_comp_gen \
	npntest \
	npnnodemgr_test \
	genpat \
	normalize \
	lrw


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@


############################################################
## npngen 用の変数の定義
############################################################

npngen_SOURCES = \
	npngen.cc \
	NpnXform.h \
	NpnXform.cc

npngen_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

npngen_LDFLAGS =


############################################################
## cannonical 用の変数の定義
############################################################

cannonical_SOURCES = \
	cannonical.cc \
	NpnXform.h \
	NpnXform.cc

cannonical_LDADD =

cannonical_LDFLAGS =


############################################################
## normalize 用の変数の定義
############################################################

normalize_SOURCES = \
	normalize.cc \
	NpnXform.h \
	NpnXform.cc

normalize_LDADD =

normalize_LDFLAGS =


############################################################
## ident 用の変数の定義
############################################################

ident_SOURCES = \
	ident.cc \
	NpnXform.h \
	NpnXform.cc

ident_LDADD =

ident_LDFLAGS =


############################################################
## read_table 用の変数の定義
############################################################

read_table_SOURCES = \
	read_table.cc \
	abc_data.cc \
	RwtMgr.h \
	RwtMgr.cc \
	RwtNode.h \
	NpnNodeMgr.h \
	NpnNodeMgr.cc \
	NppHandle.h \
	NpnNode.h \
	NpnXform.h \
	NpnXform.cc

read_table_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

read_table_LDFLAGS =


############################################################
## xform_comp_gen 用の変数の定義
############################################################

xform_comp_gen_SOURCES = \
	xform_comp_gen.cc

xform_comp_gen_LDADD =

xform_comp_gen_LDFLAGS =


############################################################
## npntest 用の変数の定義
############################################################

npntest_SOURCES = \
	NpnXform.h \
	NpnXform.cc \
	npntest.cc

npntest_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

npntest_LDFLAGS =


############################################################
## npnnodemgr_test 用の変数の定義
############################################################

npnnodemgr_test_SOURCES = \
	NpnXform.h \
	NpnXform.cc \
	NpnNode.h \
	NpnNodeMgr.h \
	NpnNodeMgr.cc \
	npnnodemgr_test.cc

npnnodemgr_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

npnnodemgr_test_LDFLAGS =


############################################################
## genpat 用の変数の定義
############################################################

genpat_SOURCES = \
	NpnXform.h \
	NpnXform.cc \
	NpnHandle.h \
	NpnNode.h \
	NpnNodeMgr.h \
	NpnNodeMgr.cc \
	GenPat.h \
	GenPat.cc \
	GenPat2.h \
	GenPat2.cc \
	GpMgr.h \
	GpMgr.cc \
	GpNode.h \
	genpat.cc \
	npn4cannon.h \
	npn4ident.h \
	npn4norm.h \
	npn4perm.h \
	npn4rep.h

genpat_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

genpat_LDFLAGS = \
	@POPT_LIBS@


############################################################
## lrw 用の変数の定義
############################################################

lrw_SOURCES = \
	AigTemplate.h \
	AigTemplate.cc \
	LrMgr.h \
	LrMgr.cc \
	LrMgr_init_table.cc \
	lrtable.h \
	lrw.cc

lrw_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

lrw_LDFLAGS = \
	@POPT_LIBS@
