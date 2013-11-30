## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_zdd_base.la


############################################################
## Variables for building libym_zdd_base.la
############################################################

libym_zdd_base_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/zdd \
	$(GMP_INCLUDES)

libym_zdd_base_la_SOURCES = \
	Zdd.cc \
	ZddMgr.cc \
	ZddMgrImpl.h \
	ZddMgrImpl.cc \
	Dumper.h \
	Dumper.cc

libym_zdd_base_la_LIBADD =

libym_zdd_base_la_LDFLAGS =

