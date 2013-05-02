## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_cell_python.la

############################################################
## libym_cell_python.la
############################################################

libym_cell_python_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

libym_cell_python_la_SOURCES = \
	pycell_init.cc \
	pycell_CellArea.cc \
	pycell_CellCapacitance.cc \
	pycell_CellTime.cc \
	pycell_Cell.cc \
	pycell_CellGroup.cc \
	pycell_CellClass.cc \
	pycell_CellLibrary.cc \
	PyCell.h \
	PyCell.cc \
	PyLibrary.h \
	PyLibrary.cc \
	PyPin.h \
	PyPin.cc \
	PyInputPin.h \
	PyInputPin.cc \
	PyOutputPin.h \
	PyOutputPin.cc \
	PyInoutPin.h \
	PyInoutPin.cc \
	PyInternalPin.h \
	PyInterlaPin.cc

libym_cell_python_la_LIBADD =

libym_cell_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
