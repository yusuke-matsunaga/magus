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

noinst_LTLIBRARIES = libym_utils_python.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## libym_utils_python.la 用の変数の定義
############################################################

libym_utils_python_la_SOURCES = \
	ympython.cc \
	pyutils_init.cc \
	pyutils_FileIDO.cc \
	pyutils_FileODO.cc \
	pyutils_FileInfo.cc \
	pyutils_FileLoc.cc \
	pyutils_FileRegion.cc \
	pyutils_ItvlMgr.cc \
	pyutils_MFSet.cc \
	pyutils_MsgType.cc \
	pyutils_MsgMgr.cc \
	pyutils_NameMgr.cc \
	PyMsgHandler.h \
	PyMsgHandler.cc \
	pyutils_USTime.cc \
	pyutils_StopWatch.cc \
	pyutils_MStopWatch.cc \
	pyutils_CombiGen.cc \
	pyutils_PermGen.cc \
	pyutils_MultiCombiGen.cc \
	pyutils_MultiPermGen.cc \
	pyutils_MultiSetCombiGen.cc \
	pyutils_MultiSetPermGen.cc \
	pyutils_RandGen.cc \
	pyutils_RandPermGen.cc \
	pyutils_RandCombiGen.cc

libym_utils_python_la_LIBADD =

libym_utils_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
