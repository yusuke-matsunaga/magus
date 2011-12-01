## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 958 2007-08-28 05:38:02Z matsunaga $

SUBDIRS = \
	ym_logic \
	expr \
	tvfunc \
	npn \
	bdd \
	sat \
	aig \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic.la


############################################################
## Variables for building libym_logic.la
############################################################

libym_logic_la_SOURCES =

libym_logic_la_LIBADD = \
	expr/libym_logic_expr.la \
	tvfunc/libym_logic_tvfunc.la \
	npn/libym_logic_npn.la \
	bdd/libym_logic_bdd.la \
	sat/libym_logic_sat.la \
	aig/libym_logic_aig.la

libym_logic_la_LDFLAGS =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_la_SOURCES = dummy.cc
