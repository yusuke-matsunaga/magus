## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	libraries \
	tests \
	programs


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = doxygen.warnings

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST = \
	BUILDING \
	Doxyfile.in \
	aclocal.m4 \
	boot.sh \
	clean.sh \
	configure \
	configure.ac \
	etc/autogen.tmpl \
	etc/autogen.common \
	etc/autotroll.mk \
	etc/common_defs \
	etc/update \
	etc/mk.subr \
	m4macros/autotroll.m4 \
	m4macros/cpputest.m4 \
	m4macros/gmp.m4
	m4macros/popt.m4 \
	m4macros/py.m4 \
	m4macros/qscintilla2.m4 \
	m4macros/qt4.m4 \
	m4macros/tcl.m4 \
	m4macros/ymlibs.m4 \
	mkautogen


############################################################
## Doxygen 用の記述
############################################################

dox:
	@DOXYGEN@
