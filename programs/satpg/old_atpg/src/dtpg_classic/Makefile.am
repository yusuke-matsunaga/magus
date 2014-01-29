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

noinst_LTLIBRARIES = \
	libatpg_dtpg_cl.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old_atpg/include


############################################################
## libatpg_dtpg_cl.la 用の変数の定義
############################################################

libatpg_dtpg_cl_la_SOURCES = \
	dtpg_opt.h \
	LqElem.h \
	Gate.h \
	Gate.cc \
	GateConst.h \
	GateConst.cc \
	GateInput.h \
	GateInput.cc \
	GateBuf.h \
	GateBuf.cc \
	GateAnd.h \
	GateAnd.cc \
	GateOr.h \
	GateOr.cc \
	GateXor.h \
	GateXor.cc \
	GateSa.h \
	GateSa.cc \
	Network.h \
	Network.cc \
	t_analyze.cc \
	LvlQ.h \
	LvlQ.cc \
	Cfna.h \
	Cfna.cc \
	backtrace.cc \
	backtrace.h \
	PGraph.cc \
	PGraph.h \
	sense.h \
	sense.cc \
	SearchMgr.h \
	SearchMgr.cc \
	DtpgCl.h \
	DtpgCl.cc

libatpg_dtpg_cl_la_LIBADD =

libatpg_dtpg_cl_la_LDFLAGS =
