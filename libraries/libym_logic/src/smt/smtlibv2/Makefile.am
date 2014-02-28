## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	parser \
	shell \
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
	libsmtlibv2.la \
	libsmtlibv2_d.la \
	libsmtlibv2_p.la


############################################################
## libsmtlibv2.la 用の変数の指定
############################################################

libsmtlibv2_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libsmtlibv2_la_SOURCES = dummy.cc

libsmtlibv2_la_LIBADD = \
	shell/libsmtshell.la \
	parser/libsmtparser.la

libsmtlibv2_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libsmtlibv2_d.la 用の変数の指定
############################################################

libsmtlibv2_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libsmtlibv2_d_la_SOURCES = dummy.cc

libsmtlibv2_d_la_LIBADD = \
	shell/libsmtshell_d.la \
	parser/libsmtparser_d.la

libsmtlibv2_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libsmtlibv2_p.la 用の変数の指定
############################################################

libsmtlibv2_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libsmtlibv2_p_la_SOURCES = dummy.cc

libsmtlibv2_p_la_LIBADD = \
	shell/libsmtshell_p.la \
	parser/libsmtparser_p.la

libsmtlibv2_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
