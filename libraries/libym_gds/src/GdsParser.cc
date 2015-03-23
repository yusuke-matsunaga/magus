
/// @file GdsParser.cc
/// @brief GdsParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsParser.h"
#include "gds/Msg.h"
#include "GdsRecTable.h"

#include "gds/GdsACL.h"
#include "gds/GdsData.h"
#include "gds/GdsDate.h"
#include "gds/GdsFormat.h"
#include "gds/GdsProperty.h"
#include "gds/GdsStrans.h"
#include "gds/GdsString.h"
#include "gds/GdsStruct.h"
#include "gds/GdsUnits.h"
#include "gds/GdsXY.h"

#include "GdsAref.h"
#include "GdsBoundary.h"
#include "GdsBox.h"
#include "GdsNode.h"
#include "GdsPath.h"
#include "GdsSref.h"
#include "GdsText.h"


#if 0
BEGIN_NAMESPACE_YM_GDS_PARSER

#include "gds_grammer.hh"

extern
int yyparse(GdsParser&);

END_NAMESPACE_YM_GDS_PARSER
#endif

BEGIN_NAMESPACE_YM_GDS

// @brief コンストラクタ
GdsParser::GdsParser() :
  mAlloc(4096)
{
}

// @brief デストラクタ
GdsParser::~GdsParser()
{
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
GdsParser::parse(const string& filename)
{
  if ( !mScanner.open_file(filename) ) {
    return false;
  }

  mCurData = NULL;
  mFormatType = 0;
  mMasks.clear();

  bool stat = true;

  if ( !read_header() ) {
    stat = false;
    goto end;
  }

  for ( ; ; ) {
    if ( !mScanner.read_rec() ) {
      stat = false;
      goto end;
    }
    if ( mScanner.cur_rtype() == kGdsBGNSTR ) {
      if ( !read_structure() ) {
	stat = false;
	goto end;
      }
    }
    else if ( mScanner.cur_rtype() == kGdsENDLIB ) {
      break;
    }
    else {
      // error
      stat = false;
      goto end;
    }
  }

 end:

  mScanner.close_file();

  return stat;
}

bool
GdsParser::read_header()
{
  if ( !mScanner.read_rec() ) {
    return false;
  }

  // HEADER: version
  if ( mScanner.cur_rtype() != kGdsHEADER ) {
    return false;
  }
  ymint16 version = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // BGNLIB
  if ( mScanner.cur_rtype() != kGdsBGNLIB ) {
    return false;
  }
  GdsDate* date = new_date();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ LIBDIRSIZE ]
  ymint16 libdirsize = 0;
  if ( mScanner.cur_rtype() == kGdsLIBDIRSIZE ) {
    libdirsize = new_int2();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ SRFNAME ]
  GdsString* srfname = NULL;
  if ( mScanner.cur_rtype() == kGdsSRFNAME ) {
    srfname = new_string();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ LIBSECUR ]
  GdsACL* acl = NULL;
  if ( mScanner.cur_rtype() == kGdsLIBSECUR ) {
    acl = new_acl();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // LIBNAME
  if ( mScanner.cur_rtype() != kGdsLIBNAME ) {
    return false;
  }
  GdsString* libname = new_string();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ REFLIBS ]
  GdsString* reflibs = NULL;
  if ( mScanner.cur_rtype() == kGdsREFLIBS ) {
    reflibs = new_string();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ FONTS ]
  GdsString* fonts = NULL;
  if ( mScanner.cur_rtype() == kGdsFONTS ) {
    fonts = new_string();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ ATTRTABLE ]
  GdsString* attrtable = NULL;
  if ( mScanner.cur_rtype() == kGdsATTRTABLE ) {
    attrtable = new_string();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ GENERATIONS ]
  ymint16 generations = 0;
  if ( mScanner.cur_rtype() == kGdsGENERATIONS ) {
    generations = new_int2();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ FORMAT | FORMAT { MASK } + ENDMASKS ]
  GdsFormat* format = NULL;
  if ( mScanner.cur_rtype() == kGdsFORMAT ) {
    ymint16 format_type = new_int2();

    if ( !mScanner.read_rec() ) {
      return false;
    }

    vector<GdsString*> masks;
    while ( mScanner.cur_rtype() == kGdsMASK ) {
      GdsString* mask = new_string();
      masks.push_back(mask);

      if ( !mScanner.read_rec() ) {
	return false;
      }
    }
    if ( !masks.empty() ) {
      if ( mScanner.cur_rtype() != kGdsENDMASKS ) {
	return false;
      }

      if ( !mScanner.read_rec() ) {
	return false;
      }
    }

    format = new_format(format_type, masks);
  }

  // UNITS
  if ( mScanner.cur_rtype() != kGdsUNITS ) {
    return false;
  }
  GdsUnits* units = new_units();

  void* p = mAlloc.get_memory(sizeof(GdsData));
  mCurData = new (p) GdsData(version, date, libdirsize, srfname, acl, libname,
			     reflibs, fonts, attrtable, generations, format, units);
  mCurStruct = NULL;

  return true;
}

bool
GdsParser::read_structure()
{
  // BGNSTR を読んだ直後
  GdsDate* date = new_date();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // STRNAME
  if ( mScanner.cur_rtype() != kGdsSTRNAME ) {
    return false;
  }

  GdsString* strname = new_string();

  void* p = mAlloc.get_memory(sizeof(GdsStruct));
  GdsStruct* str = new (p) GdsStruct(date, strname);

  if ( mCurStruct ) {
    mCurStruct->mLink = str;
  }
  else {
    mCurData->mStruct = str;
  }
  mCurStruct = str;

  mCurElement = NULL;

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ STRCLASS ]
  if ( mScanner.cur_rtype() == kGdsSTRCLASS ) {
    // あれ？これ何するの？
    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // { element }* ENDSTR
  for ( ; ; ) {
    switch ( mScanner.cur_rtype() ) {
    case kGdsENDSTR:
      if ( !mScanner.read_rec() ) {
	return false;
      }
      return true;

    case kGdsBOUNDARY:
      if ( !read_boundary() ) {
	return false;
      }
      break;

    case kGdsPATH:
      if ( !read_path() ) {
	return false;
      }
      break;

    case kGdsSREF:
      if ( !read_sref() ) {
	return false;
      }
      break;

    case kGdsAREF:
      if ( !read_aref() ) {
	return false;
      }
      break;

    case kGdsTEXT:
      if ( !read_text() ) {
	return false;
      }
      break;

    case kGdsNODE:
      if ( !read_node() ) {
	return false;
      }
      break;

    case kGdsBOX:
      if ( !read_box() ) {
	return false;
      }
      break;

    default:
      return false;
    }

    // { PROPATTR PROPVALUE }*
    for ( ; ; ) {
      if ( mScanner.cur_rtype() != kGdsPROPATTR ) {
	break;
      }
      ymint16 prop_attr = new_int2();

      if ( !mScanner.read_rec() ) {
	return false;
      }

      if ( mScanner.cur_rtype() != kGdsPROPVALUE ) {
	return false;
      }
      GdsString* prop_value = new_string();

      if ( !mScanner.read_rec() ) {
	return false;
      }

      add_property(prop_attr, prop_value);
    }

    // ENDEL
    if ( mScanner.cur_rtype() != kGdsENDEL ) {
      return false;
    }

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  ASSERT_NOT_REACHED;
  return false;
}

bool
GdsParser::read_boundary()
{
  // BOUNDARY を読んだ直後

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // LAYER
  if ( mScanner.cur_rtype() != kGdsLAYER ) {
    return false;
  }
  ymint16 layer = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // DATATYPE
  if ( mScanner.cur_rtype() != kGdsDATATYPE ) {
    return false;
  }
  ymint16 datatype = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  void* p = mAlloc.get_memory(sizeof(GdsBoundary));
  GdsBoundary* boundary = new (p) GdsBoundary(elflags, plex, layer, datatype, xy);

  add_element(boundary);

  return true;
}

bool
GdsParser::read_path()
{
  // PATH を読み込んだあと

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // LAYER
  if ( mScanner.cur_rtype() != kGdsLAYER ) {
    return false;
  }
  ymint16 layer = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // DATATYPE
  if ( mScanner.cur_rtype() != kGdsDATATYPE ) {
    return false;
  }
  ymint16 datatype = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ PATHTYPE ]
  ymint16 pathtype = 0;
  if ( mScanner.cur_rtype() == kGdsPATHTYPE ) {
    pathtype = new_int2();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ WIDTH ]
  ymint32 width = 0;
  if ( mScanner.cur_rtype() == kGdsWIDTH ) {
    width = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ BGNEXTN ]
  ymint32 bgn_extn = 0;
  if ( mScanner.cur_rtype() == kGdsBGNEXTN ) {
    bgn_extn = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ ENDEXTN ]
  ymint32 end_extn = 0;
  if ( mScanner.cur_rtype() == kGdsENDEXTN ) {
    end_extn = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  void* p = mAlloc.get_memory(sizeof(GdsPath));
  GdsPath* path = new (p) GdsPath(elflags, plex, layer, datatype, pathtype, width, bgn_extn, end_extn, xy);

  add_element(path);

  return true;
}

bool
GdsParser::read_sref()
{
  // SREF を読んだ直後

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // SNAME
  if ( mScanner.cur_rtype() != kGdsSNAME ) {
    return false;
  }
  GdsString* strname = new_string();

  // [ STRANS [ MAG ] [ ANGLE ] ]
  GdsStrans* strans = NULL;
  if ( !read_strans(strans) ) {
    return false;
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  void* p = mAlloc.get_memory(sizeof(GdsSref));
  GdsSref* sref = new (p) GdsSref(elflags, plex, strname, strans, xy);

  add_element(sref);

  return true;
}

bool
GdsParser::read_aref()
{
  // AREF を読んだ直後

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // SNAME
  if ( mScanner.cur_rtype() != kGdsSNAME ) {
    return false;
  }
  GdsString* strname = new_string();

  // [ STRANS [ MAG ] [ ANGLE ] ]
  GdsStrans* strans = NULL;
  if ( !read_strans(strans) ) {
    return false;
  }

  // COLROW
  if ( mScanner.cur_rtype() != kGdsCOLROW ) {
    return false;
  }
  ymuint32 colrow = new_colrow();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  void* p = mAlloc.get_memory(sizeof(GdsAref));
  GdsAref* aref = new (p) GdsAref(elflags, plex, strname, strans, colrow, xy);

  add_element(aref);

  return true;
}

bool
GdsParser::read_text()
{
  // TEXT を読んだ直後

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // LAYER
  if ( mScanner.cur_rtype() != kGdsLAYER ) {
    return false;
  }
  ymint16 layer = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // TEXTTYPE
  if ( mScanner.cur_rtype() != kGdsTEXTTYPE ) {
    return false;
  }
  ymint16 texttype = new_int2();

  // [ PRESENTATION ]
  ymuint16 presentation = 0;
  if ( mScanner.cur_rtype() == kGdsPRESENTATION ) {
    presentation = new_int2();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PATHTYPE ]
  ymint16 pathtype = 0;
  if ( mScanner.cur_rtype() == kGdsPATHTYPE ) {
    pathtype = new_int2();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ WIDTH ]
  ymint32 width = 0;
  if ( mScanner.cur_rtype() == kGdsWIDTH ) {
    width = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ STRANS [ MAG ] [ ANGLE ] ]
  GdsStrans* strans = NULL;
  if ( !read_strans(strans) ) {
    return false;
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  // TEXT
  if ( mScanner.cur_rtype() != kGdsTEXT ) {
    return false;
  }
  GdsString* body = new_string();

  void* p = mAlloc.get_memory(sizeof(GdsText));
  GdsText* text = new (p) GdsText(elflags, plex, layer, texttype, presentation, pathtype, width, strans, xy, body);

  add_element(text);

  return true;
}

bool
GdsParser::read_node()
{
  // NODE を読んだ直後

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // LAYER
  if ( mScanner.cur_rtype() != kGdsLAYER ) {
    return false;
  }
  ymint16 layer = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // NODETYPE
  if ( mScanner.cur_rtype() != kGdsNODETYPE ) {
    return false;
  }
  ymint16 nodetype = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  void* p = mAlloc.get_memory(sizeof(GdsNode));
  GdsNode* node = new (p) GdsNode(elflags, plex, layer, nodetype, xy);

  add_element(node);

  return true;
}

bool
GdsParser::read_box()
{
  // BOX を読んだ直後

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ ELFLAGS ]
  ymuint16 elflags = 0U;
  if ( mScanner.cur_rtype() == kGdsELFLAGS ) {
    elflags = new_bitarray();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ PLEX ]
  ymint32 plex = 0;
  if ( mScanner.cur_rtype() == kGdsPLEX ) {
    plex = new_int4();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // LAYER
  if ( mScanner.cur_rtype() != kGdsLAYER ) {
    return false;
  }
  ymint16 layer = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // BOXTYPE
  if ( mScanner.cur_rtype() != kGdsBOXTYPE ) {
    return false;
  }
  ymint16 boxtype = new_int2();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // XY
  if ( mScanner.cur_rtype() != kGdsXY ) {
    return false;
  }
  GdsXY* xy = new_xy();

  void* p = mAlloc.get_memory(sizeof(GdsBox));
  GdsBox* box = new (p) GdsBox(elflags, plex, layer, boxtype, xy);

  add_element(box);

  return true;
}

// @brief STRANS 以降の読み込み
// @param[out] strans 結果を格納する変数
//
// エラーが起きたら false を返す．
bool
GdsParser::read_strans(GdsStrans*& strans)
{
  if ( mScanner.cur_rtype() != kGdsSTRANS ) {
    return true;
  }

  // STRANS を読み込んだ直後

  ymuint flags = new_bitarray();

  if ( !mScanner.read_rec() ) {
    return false;
  }

  // [ MAG ]
  double mag = 1.0;
  if ( mScanner.cur_rtype() == kGdsMAG ) {
    mag = new_real();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  // [ ANGLE ]
  double angle = 0.0;
  if ( mScanner.cur_rtype() == kGdsANGLE ) {
    angle = new_real();

    if ( !mScanner.read_rec() ) {
      return false;
    }
  }

  void* p = mAlloc.get_memory(sizeof(GdsStrans));
  strans = new (p) GdsStrans(flags, mag, angle);

  return true;
}

// @brief GdsElement (の派生要素)の追加
void
GdsParser::add_element(GdsElement* elem)
{
  if ( mCurElement ) {
    mCurElement->mLink = elem;
  }
  else {
    mCurStruct->mElement = elem;
  }
  mCurElement = elem;

  mCurProperty = NULL;
}

// @brief GdsProperty の作成
// @param[in] attr PROPATTR の値
// @param[in] value PROPVALUE の値
void
GdsParser::add_property(ymuint attr,
			GdsString* value)
{
  void* p = mAlloc.get_memory(sizeof(GdsProperty));
  GdsProperty* prop = new (p) GdsProperty(attr, value);

  if ( mCurProperty ) {
    mCurProperty->mLink = prop;
  }
  else {
    mCurElement->mProperty = prop;
  }
  mCurProperty = prop;
}

#if 0
// @brief yylex() の実装
int
GdsParser::yylex(nsParser::YYSTYPE& lval)
{
  bool stat = mScanner.read_rec();
  if ( !stat ) {
    // エラー
    return 0;
  }

  const GdsRecTable& table = GdsRecTable::obj();

  GdsRtype rtype = mScanner.cur_rtype();
  switch ( rtype ) {
  case kGdsBGNLIB:        lval.date_type = new_date(); break;
  case kGdsUNITS:         lval.units_type = new_units(); break;
  case kGdsBGNSTR:        lval.date_type = new_date(); break;
  case kGdsXY:            lval.xy_type = new_xy(); break;
  case kGdsCOLROW:        lval.colrow_type = new_colrow(); break;
  case kGdsPRESENTATION:  lval.bitarray_type = new_bitarray(); break;
  case kGdsSTRANS:        lval.bitarray_type = new_bitarray(); break;
  case kGdsELFLAGS:       lval.bitarray_type = new_bitarray(); break;
  case kGdsLIBSECUR:      lval.acl_type = new_acl(); break;
  default:
    ASSERT_COND( table.data_num(rtype) <= 1 );
    {
      GdsDtype dtype = mScanner.cur_dtype();
      switch ( dtype ) {
      case kGds2Int:   lval.int2_type = new_int2(); break;
      case kGds4Int:   lval.int4_type = new_int4(); break;
      case kGds8Real:  lval.real_type = new_real(); break;
      case kGdsString: lval.string_type = new_string(); break;
      case kGdsNodata: break;
      default:
	ASSERT_NOT_REACHED;
	break;
      }
    }
    break;
  }

  int token = table.rtype_token(rtype);
  return token;
}
#endif

// @brief yyerror() の実装
int
GdsParser::yyerror(const char* s)
{
  error_header(__FILE__, __LINE__, "GdsParser", mScanner.cur_pos(), s);
  msg_end();

  return 1;
}

// @brief GdsFormat の作成
GdsFormat*
GdsParser::new_format(ymint16 format_type,
		      const vector<GdsString*>& masks)
{
  if ( format_type == 0 && masks.empty() ) {
    return NULL;
  }
  ymuint nm = masks.size();
  void* p = mAlloc.get_memory(sizeof(GdsFormat) + (nm - 1) * sizeof(GdsString*));
  GdsFormat* format = new (p) GdsFormat(format_type);
  format->mMaskNum = nm;
  for (ymuint i = 0; i < nm; ++ i) {
    format->mMasks[i] = masks[i];
  }
  return format;
}

// @brief GdsACL の作成
GdsACL*
GdsParser::new_acl()
{
  ymuint dsize = mScanner.cur_dsize();
  ASSERT_COND( dsize % 6 == 0 );
  ymuint n = dsize / 6;
  GdsACL* top = NULL;
  GdsACL** pprev = &top;
  for (ymuint i = 0; i < n; ++ i) {
    ymuint group = mScanner.conv_2byte_int(i * 3 + 0);
    ymuint user = mScanner.conv_2byte_int(i * 3 + 1);
    ymuint access = mScanner.conv_2byte_int(i * 3 + 2);
    void* p = mAlloc.get_memory(sizeof(GdsACL));
    GdsACL* acl = new (p) GdsACL(group, user, access);
    *pprev = acl;
    pprev = &acl->mNext;
  }
  *pprev = NULL;

  return top;
}

// @brief COLROW の作成
ymuint32
GdsParser::new_colrow()
{
  ymint col = mScanner.conv_2byte_int(0);
  ymint row = mScanner.conv_2byte_int(1);
  return (static_cast<ymuint32>(col) << 16) | static_cast<ymuint32>(row);
}

// @brief GdsDate の作成
GdsDate*
GdsParser::new_date()
{
  void* p = mAlloc.get_memory(sizeof(GdsDate[2]));
  GdsDate* date = new (p) GdsDate[2];
  ymuint year1 = mScanner.conv_2byte_int(0);
  ymuint month1 = mScanner.conv_2byte_int(1);
  ymuint day1 = mScanner.conv_2byte_int(2);
  ymuint hour1 = mScanner.conv_2byte_int(3);
  ymuint min1 = mScanner.conv_2byte_int(4);
  ymuint sec1 = mScanner.conv_2byte_int(5);
  date[0].set(year1, month1, day1, hour1, min1, sec1);

  ymuint year2 = mScanner.conv_2byte_int(6);
  ymuint month2 = mScanner.conv_2byte_int(7);
  ymuint day2 = mScanner.conv_2byte_int(8);
  ymuint hour2 = mScanner.conv_2byte_int(9);
  ymuint min2 = mScanner.conv_2byte_int(10);
  ymuint sec2 = mScanner.conv_2byte_int(11);
  date[1].set(year2, month2, day2, hour2, min2, sec2);

  return date;
}

// @brief GdsString の作成
GdsString*
GdsParser::new_string()
{
  char* src_str = reinterpret_cast<char*>(mScanner.cur_data());
  ymuint len = mScanner.cur_dsize();
  for (ymuint i = 0; i < len; ++ i) {
    if ( src_str[i] == '\0' ) {
      len = i;
      break;
    }
  }

  void* p = mAlloc.get_memory(sizeof(GdsString) + len);
  GdsString* str = new (p) GdsString;

  for (ymuint i = 0; i < len; ++ i) {
    str->mStr[i] = src_str[i];
  }
  str->mStr[len] = '\0';

  return str;
}

// @brief GdsUnits の作成
GdsUnits*
GdsParser::new_units()
{
  double user = mScanner.conv_8byte_real(0);
  double meter = mScanner.conv_8byte_real(1);

  void* p = mAlloc.get_memory(sizeof(GdsUnits));
  GdsUnits* units = new (p) GdsUnits(user, meter);

  return units;
}

// @brief GdsXY の作成
GdsXY*
GdsParser::new_xy()
{
  ymuint dsize = mScanner.cur_dsize();
  ASSERT_COND( dsize % 8 == 0 );
  ymuint num = dsize / 4;

  void* p = mAlloc.get_memory(sizeof(GdsXY) + sizeof(ymint32) * (num - 1));
  GdsXY* xy = new (p) GdsXY();

  xy->mNum = num / 2;
  for (ymuint i = 0; i < num; ++ i) {
    xy->mData[i] = mScanner.conv_4byte_int(i);
  }

  return xy;
}

// @brief int2 の作成
ymint16
GdsParser::new_int2()
{
  return mScanner.conv_2byte_int(0);
}

// @brief int4 の作成
ymint32
GdsParser::new_int4()
{
  return mScanner.conv_4byte_int(0);
}

// @brief real の作成
double
GdsParser::new_real()
{
  return mScanner.conv_8byte_real(0);
}

// @brief BitArray の作成
ymuint16
GdsParser::new_bitarray()
{
  return static_cast<ymuint16>(mScanner.conv_2byte_int(0));
}

END_NAMESPACE_YM_GDS
