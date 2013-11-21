## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 958 2007-08-28 05:38:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic_expr.la


############################################################
## Variables for building libym_logic_expr.la
############################################################

libym_logic_expr_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_logic_expr_la_SOURCES = \
	lexp_types.h \
	LexpMgr.h \
	LexpNode.h \
	LexpNodePtr.h \
	LexpParser.h \
	SopLit.h \
	Literal.cc \
	LexpMgr.cc \
	LexpNode.cc \
	LexpParser.cc \
	LogExpr.cc \
	LogExprWriter.cc

libym_logic_expr_la_LIBADD =

libym_logic_expr_la_LDFLAGS =
