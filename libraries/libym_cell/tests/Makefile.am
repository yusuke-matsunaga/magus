## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = . \
	  $(YM_TESTS_SUBDIRS)


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

bin_PROGRAMS = \
	mislib_parser_test \
	mislib_reader_test \
	dotlib_parser_test \
	dotlib_reader_test \
	dump_restore_test \
	cell_mgr_test \
	cell_dump_test \
	luttest \
	lcpatmgr_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell


############################################################
## 変数定義
############################################################

YM_CELL = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## mislib_parser_test 用の変数の指定
############################################################

mislib_parser_test_SOURCES = \
	mislib_parser_test.cc

mislib_parser_test_LDADD = \
	$(YM_CELL)

mislib_parser_test_LDFLAGS =


############################################################
## mislib_reader_test 用の変数の指定
############################################################

mislib_reader_test_SOURCES = \
	mislib_reader_test.cc

mislib_reader_test_LDADD = \
	$(YM_CELL)

mislib_reader_test_LDFLAGS =


############################################################
## dotlist_reader_test 用の変数の指定
############################################################

dotlib_reader_test_SOURCES = \
	dotlib_reader_test.cc

dotlib_reader_test_LDADD = \
	$(YM_CELL)

dotlib_reader_test_LDFLAGS =


############################################################
## luttest 用の変数の指定
############################################################

luttest_SOURCES = \
	luttest.cc

luttest_LDADD = \
	$(YM_CELL)

luttest_LDFLAGS =


############################################################
## lcpatmgr_test 用の変数の指定
############################################################

lcpatmgr_test_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/libcomp

lcpatmgr_test_SOURCES = \
	lcpatmgr_test.cc

lcpatmgr_test_LDADD = \
	$(YM_CELL)

lcpatmgr_test_LDFLAGS =


############################################################
## dotlib_parser_test 用の変数の指定
############################################################

dotlib_parser_test_SOURCES = \
	dotlib_parser_test.cc

dotlib_parser_test_LDADD = \
	$(YM_CELL)

dotlib_parser_test_LDFLAGS =


############################################################
## dump_restore_test 用の変数の指定
############################################################

dump_restore_test_SOURCES = \
	dump_restore_test.cc

dump_restore_test_LDADD = \
	$(YM_CELL)

dump_restore_test_LDFLAGS =


############################################################
## cell_mgr_test 用の変数の指定
############################################################

cell_mgr_test_SOURCES = \
	cell_mgr_test.cc

cell_mgr_test_LDADD = \
	$(YM_CELL)

cell_mgr_test_LDFLAGS =


############################################################
## cell_dump_test 用の変数の指定
############################################################

cell_dump_test_SOURCES = \
	cell_dump_test.cc

cell_dump_test_LDADD = \
	$(YM_CELL)


#libdump_test_SOURCES = \
#	libdump_test.cc

#libdump_test_LDADD = \
#	$(YM_CELL)
