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

EXTRA_DIST = \
	atpg_utils/pkgIndex.tcl \
	atpg_utils/scripts/*.tcl \
	atpg_utils/scripts/tclIndex


############################################################
## 独自の make ルールの追加
############################################################

TCLSH = ${top_builddir}/old_atpg/src/atpg_tclsh/atpg_tclsh
SETUP = ${top_srcdir}/old_atpg/utils/tclpkg-setup.tcl
INSTALL = ${top_srcdir}/old_atpg/utils/tclpkg-install.tcl
PKGSRC = ${top_srcdir}/old_atpg/tclsrc

all-local :
	$(TCLSH) $(SETUP) $(PKGSRC) "{atpg_utils}"

install-data-local :
	$(TCLSH) $(INSTALL) $(PKGSRC) $(ATPG_LIBDIR) "{atpg_utils}"
