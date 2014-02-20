## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include

YM_BASE = $(top_builddir)/../libym_utils/src/libym_utils.la

noinst_PROGRAMS = \
	symtablegen \
	tblgen \
	npn_tablegen \
	mpx

symtablegen_SOURCES = \
	symtablegen.cc

tblgen_SOURCES = \
	tblgen.cc

npn_tablegen_SOURCES = \
	npn_tablegen.cc
npn_tablegen_LDADD = \
	$(YM_BASE)

mpx_SOURCES = \
	mpx.cc
mpx_LDADD =
