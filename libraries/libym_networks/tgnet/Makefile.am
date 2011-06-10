## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_tgnet.la

libym_tgnet_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

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

libym_tgnet_la_LIBADD =

libym_tgnet_la_LDFLAGS =
