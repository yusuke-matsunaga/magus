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
	libio.la \
	libio_d.la \
	libio_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	ZCoder.h \
	ZCoder.cc \
	ZDecoder.h \
	ZDecoder.cc \
	FileBuff.h \
	FileBuff.cc \
	FileCoder.cc \
	FileDecoder.cc \
	FileIDO.cc \
	FileODO.cc \
	IDO.cc \
	ODO.cc \
	ThruCoder.h \
	ThruCoder.cc \
	ThruDecoder.h \
	ThruDecoder.cc

if YM_HAVE_ZLIB
COMMON_SOURCES += \
	zstream.h \
	zstream.cc \
	GzCoder.h \
	GzCoder.cc \
	GzDecoder.h \
	GzDecoder.cc

AM_LDFLAGS += \
	-lz
endif

if YM_HAVE_BZLIB
COMMON_SOURCES += \
	BzEngine.h \
	BzCoder.h \
	BzCoder.cc \
	BzDecoder.h \
	BzDecoder.cc


AM_LDFLAGS += \
	-lbz2
endif

if YM_HAVE_LZMA
COMMON_SOURCES += \
	LzmaEngine.h \
	LzmaCoder.h \
	LzmaCoder.cc \
	LzmaDecoder.h \
	LzmaDecoder.cc

AM_LDFLAGS += \
	-llzma
endif


############################################################
## libio.la 用の変数の定義
############################################################

libio_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libio_la_SOURCES = $(COMMON_SOURCES)

libio_la_LIBADD = \
	$(AM_LIBADD)

libio_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libio_d.la 用の変数の定義
############################################################

libio_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libio_d_la_SOURCES = $(COMMON_SOURCES)

libio_d_la_LIBADD = \
	$(AM_LIBADD)

libio_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libio_p.la 用の変数の定義
############################################################

libio_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libio_p_la_SOURCES = $(COMMON_SOURCES)

libio_p_la_LIBADD = \
	$(AM_LIBADD)

libio_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
