## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_logic_python.la

############################################################
## libym_logic_python.la
############################################################

libym_logic_python_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES) \
	$(PYTHON_CPPFLAGS)

libym_logic_python_la_SOURCES = \
	pylogic_init.cc \
	pylogic_Bool3.cc \
	pylogic_VarId.cc \
	pylogic_Pol.cc \
	pylogic_Literal.cc \
	pylogic_LogExpr.cc \
	pylogic_BddMgr.cc \
	pylogic_Bdd.cc \
	pylogic_AigMgr.cc \
	pylogic_Aig.cc \
	pylogic_SatSolver.cc

libym_logic_python_la_LIBADD =

libym_logic_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
