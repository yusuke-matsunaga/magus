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

noinst_LTLIBRARIES = libym_logic_python.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES) \
	$(PYTHON_CPPFLAGS)


############################################################
## libym_logic_python.la 用の変数の指定
############################################################

libym_logic_python_la_SOURCES = \
	pylogic_init.cc \
	pylogic_Bool3.cc \
	pylogic_VarId.cc \
	pylogic_Pol.cc \
	pylogic_Literal.cc \
	pylogic_TvFunc.cc \
	pylogic_LogExpr.cc \
	pylogic_BddMgr.cc \
	pylogic_Bdd.cc \
	pylogic_AigMgr.cc \
	pylogic_Aig.cc \
	pylogic_SatSolver.cc \
	PySatMsgHandler.h \
	PySatMsgHandler.cc

libym_logic_python_la_LIBADD =

libym_logic_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
