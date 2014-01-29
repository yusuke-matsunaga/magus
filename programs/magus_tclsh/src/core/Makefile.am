## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


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

noinst_LTLIBRARIES = libmagus_core_tcl.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## libmagus_core_tcl.la 用の変数の定義
############################################################

libmagus_core_tcl_la_SOURCES = \
	MagMgr.cc \
	NetHandle.cc \
	BNetHandle.h \
	BNetHandle.cc \
	BdnHandle.h \
	BdnHandle.cc \
	MvnHandle.h \
	MvnHandle.cc \
	MagCmd.cc \
	NetCmd.cc \
	NewBNetwork.h \
	NewBNetwork.cc \
	NewBdn.h \
	NewBdn.cc \
	NewMvn.h \
	NewMvn.cc \
	ClrNetwork.h \
	ClrNetwork.cc \
	DelNetwork.h \
	DelNetwork.cc \
	CopyNetwork.h \
	CopyNetwork.cc \
	ChgNetwork.h \
	ChgNetwork.cc \
	PushNetwork.h \
	PushNetwork.cc \
	PopNetwork.h \
	PopNetwork.cc \
	CurNetwork.h \
	CurNetwork.cc \
	ListNetwork.h \
	ListNetwork.cc \
	ForNetwork.h \
	ForNetwork.cc \
	ReadBlif.h \
	ReadBlif.cc \
	ReadIscas89.h \
	ReadIscas89.cc \
	WriteBlif.h \
	WriteBlif.cc \
	WriteVerilog.h \
	WriteVerilog.cc \
	ReadLibrary.h \
	ReadLibrary.cc \
	ReadMislib.h \
	ReadMislib.cc \
	ReadLiberty.h \
	ReadLiberty.cc \
	RestoreLibrary.h \
	RestoreLibrary.cc \
	DumpLibrary.h \
	DumpLibrary.cc \
	core_init.cc

libmagus_core_tcl_la_LIBADD =

libmagus_core_tcl_la_LDFLAGS =
