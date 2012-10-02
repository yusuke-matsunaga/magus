## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_logic_pylib.la

############################################################
## libym_logic_pylib.la
############################################################

libym_logic_pylib_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES) \
	$(PYTHON_CPPFLAGS)

libym_logic_pylib_la_SOURCES = \
	logicpy.h \
	logicpy_init.cc \
	logicpy_VarId.cc \
	logicpy_Pol.cc \
	logicpy_Literal.cc \
	logicpy_LogExpr.cc \
	logicpy_BddMgr.cc \
	logicpy_Bdd.cc \
	logicpy_AigMgr.cc \
	logicpy_Aig.cc \
	logicpy_SatSolver.cc

libym_logic_pylib_la_LIBADD =

libym_logic_pylib_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
