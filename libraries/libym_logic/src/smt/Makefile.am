## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	smtlibv2 \
	core \
	.

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
	libsmt.la \
	libsmt_d.la \
	libsmt_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS =


############################################################
## libsmt.la 用の変数の指定
############################################################

libsmt_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libsmt_la_SOURCES = dummy.cc

libsmt_la_LIBADD = \
	core/libsmtcore.la \
	smtlibv2/libsmtlibv2.la

libsmt_la_LDFLAGS =


############################################################
## libsmt_d.la 用の変数の指定
############################################################

libsmt_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libsmt_d_la_SOURCES = dummy.cc

libsmt_d_la_LIBADD = \
	core/libsmtcore_d.la \
	smtlibv2/libsmtlibv2_d.la

libsmt_d_la_LDFLAGS =


############################################################
## libsmt_p.la 用の変数の指定
############################################################

libsmt_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libsmt_p_la_SOURCES = dummy.cc

libsmt_p_la_LIBADD = \
	core/libsmtcore_p.la \
	smtlibv2/libsmtlibv2_p.la

libsmt_p_la_LDFLAGS =
