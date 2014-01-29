## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = . \
	vltest \
	$(YM_TESTS_SUBDIRS)


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	bvcalc.cc bvcalc.h bvcalc.output bvlex.cc

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

bin_PROGRAMS = \
	bvcalc

noinst_PROGRAMS = \
	testgen \
	scalarval_test \
	udpval_test \
	valuetype_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

AM_YFLAGS=-d -v


#############################################################
# bvcalc 用の変数の定義
#############################################################

bvcalc_SOURCES = \
	bvcalc.yy \
	bvlex.ll

bvcalc_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

bvcalc_LDFLAGS =


#############################################################
# testgen 用の変数の定義
#############################################################

testgen_SOURCES = \
	testgen.cc

testgen_LDADD =

testgen_LDFLAGS =


#############################################################
# scalarval_test 用の変数の定義
#############################################################

scalarval_test_SOURCES = \
	scalarval_test.cc

scalarval_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

scalarval_test_LDFLAGS =


#############################################################
# udpval_test 用の変数の定義
#############################################################

udpval_test_SOURCES = \
	udpval_test.cc

udpval_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

udpval_test_LDFLAGS =


#############################################################
# valuetype_test 用の変数の定義
#############################################################

valuetype_test_SOURCES = \
	valuetype_test.cc

valuetype_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

valuetype_test_LDFLAGS =
