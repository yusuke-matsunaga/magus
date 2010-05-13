## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS = \
	ym_tgnet \
	. \
	tests

INCLUDES = \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libym_tgnet.la

libym_tgnet_la_SOURCES = \
	NameHash.h \
	NameHash.cc \
	LogicMgr.h \
	LogicMgr.cc \
	TgNetwork.cc \
	TgBlifHandler.h \
	TgBlifHandler.cc \
	TgBlifReader.cc \
	TgIscas89Handler.h \
	TgIscas89Handler.cc \
	TgIscas89Reader.cc

libym_tgnet_la_LDFLAGS =

libym_tgnet_la_LIBADD = \
	$(top_builddir)/libym_blif/libym_blif.la \
	$(top_builddir)/libym_iscas89/libym_iscas89.la \
	$(top_builddir)/libym_lexp/libym_lexp.la \
	$(top_builddir)/libym_utils/libym_utils.la

