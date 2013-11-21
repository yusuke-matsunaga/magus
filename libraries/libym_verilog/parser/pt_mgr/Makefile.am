## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_parser_pt_mgr.la


############################################################
## libym_vl_parser.la
############################################################

libym_vl_parser_pt_mgr_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/parser/cpt \
	-I$(top_srcdir)/parser/spt

libym_vl_parser_pt_mgr_la_SOURCES = \
	PtiFactory.cc \
	PtMgr.cc \
	PtiBase.cc \
	PtDumper.cc

libym_vl_parser_pt_mgr_la_LIBADD =

libym_vl_parser_pt_mgr_la_LDFLAGS =
