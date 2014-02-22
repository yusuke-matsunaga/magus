## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	common \
	parser \
	elaborator


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
	libym_verilog.la \
	libym_verilog_d.la \
	libym_verilog_p.la


############################################################
## libym_verilog.la 用の変数の定義
############################################################

libym_verilog_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_verilog_la_SOURCES = dummy.cc

libym_verilog_la_LIBADD = \
	common/libvlcommon.la \
	parser/libvlparser.la \
	elaborator/libvlelab.la

libym_verilog_la_LDFLAGS = \
	$(top_builddir)/../libym_cell/src/libym_cell.la \
	$(top_builddir)/../libym_utils/src/libym_utils.la


############################################################
## libym_verilog_d.la 用の変数の定義
############################################################

libym_verilog_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_verilog_d_la_SOURCES = dummy.cc

libym_verilog_d_la_LIBADD = \
	common/libvlcommon_d.la \
	parser/libvlparser_d.la \
	elaborator/libvlelab_d.la

libym_verilog_d_la_LDFLAGS =
	$(top_builddir)/../libym_cell/src/libym_cell_d.la \
	$(top_builddir)/../libym_utils/src/libym_utils_d.la


############################################################
## libym_verilog_p.la 用の変数の定義
############################################################

libym_verilog_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_verilog_p_la_SOURCES = dummy.cc

libym_verilog_p_la_LIBADD = \
	common/libvlcommon_p.la \
	parser/libvlparser_p.la \
	elaborator/libvlelab_p.la

libym_verilog_p_la_LDFLAGS =
	$(top_builddir)/../libym_cell/src/libym_cell_p.la \
	$(top_builddir)/../libym_utils/src/libym_utils_p.la
