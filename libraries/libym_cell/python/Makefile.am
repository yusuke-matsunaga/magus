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

noinst_LTLIBRARIES = libym_cell_python.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)


############################################################
## libym_cell_python.la
############################################################

libym_cell_python_la_SOURCES = \
	pycell_init.cc \
	pycell_CellLibrary.cc \
	pycell_CellClass.cc \
	pycell_CellGroup.cc \
	pycell_Cell.cc \
	pycell_CellPin.cc \
	pycell_CellTechnology.cc \
	pycell_CellDelayModel.cc \
	pycell_CellPatType.cc \
	pycell_CellPatGraph.cc \
	pycell_CellTiming.cc \
	pycell_CellTimingSense.cc \
	pycell_CellTimingType.cc \
	pycell_CellVarType.cc \
	pycell_CellLut.cc \
	pycell_CellPinDir.cc \
	pycell_CellArea.cc \
	pycell_CellCapacitance.cc \
	pycell_CellResistance.cc \
	pycell_CellTime.cc

libym_cell_python_la_LIBADD =

libym_cell_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
