
/// @file GdsParser.cc
/// @brief GdsParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsParser.h"
#include "ym_gds/Msg.h"
#include "GdsRecTable.h"

#include "ym_gds/GdsACL.h"
#include "ym_gds/GdsData.h"
#include "ym_gds/GdsDate.h"
#include "ym_gds/GdsFormat.h"
#include "ym_gds/GdsProperty.h"
#include "ym_gds/GdsStrans.h"
#include "ym_gds/GdsString.h"
#include "ym_gds/GdsStruct.h"
#include "ym_gds/GdsUnits.h"
#include "ym_gds/GdsXY.h"

#include "GdsAref.h"
#include "GdsBoundary.h"
#include "GdsBox.h"
#include "GdsNode.h"
#include "GdsPath.h"
#include "GdsSref.h"
#include "GdsText.h"


BEGIN_NAMESPACE_YM_GDS_PARSER

#include "gds_grammer.hh"

extern
int yyparse(GdsParser&);

END_NAMESPACE_YM_GDS_PARSER


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

  nsParser::yyparse(*this);

  mScanner.close_file();

  return true;
}

// @brief GdsData の生成
// @param[in] version バージョン番号
// @param[in] date 2つの日時の配列
// @param[in] libdirsize LIBDIRSIZE の値
// @param[in] srfname SRFNAME の値
// @param[in] acl LIBSECURE の値
// @param[in] libname LIBNAME の値
// @param[in] reflibs REFLIBS の値
// @param[in] fonts FONTS の値
// @param[in] attrtable ATTRTABLE の値
// @param[in] generations GENERATIONS の値
// @param[in] units UNITS の値
void
GdsParser::new_header(ymint16 version,
		      GdsDate* date,
		      ymint16 libdirsize,
		      GdsString* srfname,
		      GdsACL* acl,
		      GdsString* libname,
		      GdsString* reflibs,
		      GdsString* fonts,
		      GdsString* attrtable,
		      ymint16 generations,
		      GdsUnits* units)
{
  GdsFormat* format = new_format();

  void* p = mAlloc.get_memory(sizeof(GdsData));
  mCurData = new (p) GdsData(version, date, libdirsize, srfname, acl, libname,
			     reflibs, fonts, attrtable, generations, format, units);
  mCurStruct = NULL;
}

// @brief フォーマットタイプの設定
// @param[in] type フォーマットタイプ
void
GdsParser::set_format(ymint16 type)
{
  mFormatType = type;
}

// @brief マスクの追加
// @param[in] mask マスク
void
GdsParser::add_mask(GdsString* mask)
{
  mMasks.push_back(mask);
}

// @brief GdsStruct の追加
// @param[in] date 2つの日時の配列
// @param[in] strname 構造名
void
GdsParser::add_struct(GdsDate* date,
		      GdsString* strname)
{
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
}

// @brief GdsAref の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] strname 構造名
// @param[in] strans STRANS の値
// @param[in] colrow 列と行の数
// @param[in] xy 座標
void
GdsParser::add_aref(ymuint16 elflags,
		    ymint32 plex,
		    GdsString* strname,
		    GdsStrans* strans,
		    ymuint32 colrow,
		    GdsXY* xy)
{
  void* p = mAlloc.get_memory(sizeof(GdsAref));
  GdsAref* aref = new (p) GdsAref(elflags, plex, strname, strans, colrow, xy);

  add_element(aref);
}

// @brief GdsBoundary の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] xy XY の値
void
GdsParser::add_boundary(ymuint16 elflags,
			ymint32 plex,
			ymint16 layer,
			ymint16 datatype,
			GdsXY* xy)
{
  void* p = mAlloc.get_memory(sizeof(GdsBoundary));
  GdsBoundary* boundary = new (p) GdsBoundary(elflags, plex, layer, datatype, xy);

  add_element(boundary);
}

// @brief GdsBox の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] boxtype BOXTYPE の値
// @param[in] xy XY の値
void
GdsParser::add_box(ymuint16 elflags,
		   ymint32 plex,
		   ymint16 layer,
		   ymint16 datatype,
		   GdsXY* xy)
{
  void* p = mAlloc.get_memory(sizeof(GdsBox));
  GdsBox* box = new (p) GdsBox(elflags, plex, layer, datatype, xy);

  add_element(box);
}

// @brief GdsNode の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] xy XY の値
void
GdsParser::add_node(ymuint16 elflags,
		    ymint32 plex,
		    ymint16 layer,
		    ymint16 nodetype,
		    GdsXY* xy)
{
  void* p = mAlloc.get_memory(sizeof(GdsNode));
  GdsNode* node = new (p) GdsNode(elflags, plex, layer, nodetype, xy);

  add_element(node);
}

// @brief GdsPath の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] pathtype PATYTYPE の値
// @param[in] width WIDTH の値
// @param[in] bgn_extn BGNEXTN の値
// @param[in] end_extn ENDEXTN の値
// @param[in] xy XY の値
void
GdsParser::add_path(ymuint16 elflags,
		    ymint32 plex,
		    ymint16 layer,
		    ymint16 datatype,
		    ymint16 pathtype,
		    ymint32 width,
		    ymint32 bgn_extn,
		    ymint32 end_extn,
		    GdsXY* xy)
{
  void* p = mAlloc.get_memory(sizeof(GdsPath));
  GdsPath* path = new (p) GdsPath(elflags, plex, layer, datatype, pathtype, width, bgn_extn, end_extn, xy);

  add_element(path);
}

// @brief GdsSref の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] strname 構造名
// @param[in] strans STRANS の値
// @param[in] xy 座標
void
GdsParser::add_sref(ymuint16 elflags,
		    ymint32 plex,
		    GdsString* strname,
		    GdsStrans* strans,
		    GdsXY* xy)
{
  void* p = mAlloc.get_memory(sizeof(GdsSref));
  GdsSref* sref = new (p) GdsSref(elflags, plex, strname, strans, xy);

  add_element(sref);
}

// @brief GdsText の作成
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] texttype TEXTTYPE の値
// @param[in] presentation PRESENTATION の値
// @param[in] pathtype PATHTYPE の値
// @param[in] width WIDTH の値
// @param[in] strans STRANS の値
// @param[in] xy XY座標
// @param[in] body 本体の文字列
void
GdsParser::add_text(ymuint16 elflags,
		    ymint32 plex,
		    ymint16 layer,
		    ymint16 texttype,
		    ymuint16 presentation,
		    ymint16 pathtype,
		    ymint32 width,
		    GdsStrans* strans,
		    GdsXY* xy,
		    GdsString* body)
{
  void* p = mAlloc.get_memory(sizeof(GdsText));
  GdsText* text = new (p) GdsText(elflags, plex, layer, texttype, presentation, pathtype, width, strans, xy, body);

  add_element(text);
}

// @brief GdsStrans の作成
GdsStrans*
GdsParser::new_strans(ymuint flags,
		      double mag,
		      double angle)
{
  void* p = mAlloc.get_memory(sizeof(GdsStrans));
  GdsStrans* strans = new (p) GdsStrans(flags, mag, angle);

  return strans;
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

  tGdsRtype rtype = mScanner.cur_rtype();
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
    assert_cond( table.data_num(rtype) <= 1, __FILE__, __LINE__);
    {
      tGdsDtype dtype = mScanner.cur_dtype();
      switch ( dtype ) {
      case kGds2Int:   lval.int2_type = new_int2(); break;
      case kGds4Int:   lval.int4_type = new_int4(); break;
      case kGds8Real:  lval.real_type = new_real(); break;
      case kGdsString: lval.string_type = new_string(); break;
      case kGdsNodata: break;
      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
    }
    break;
  }

  int token = table.rtype_token(rtype);
  return token;
}

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
GdsParser::new_format()
{
  if ( mFormatType == 0 && mMasks.empty() ) {
    return NULL;
  }
  ymuint nm = mMasks.size();
  void* p = mAlloc.get_memory(sizeof(GdsFormat) + (nm - 1) * sizeof(GdsString*));
  GdsFormat* format = new (p) GdsFormat(mFormatType);
  format->mMaskNum = nm;
  for (ymuint i = 0; i < nm; ++ i) {
    format->mMasks[i] = mMasks[i];
  }
  return format;
}

// @brief GdsACL の作成
GdsACL*
GdsParser::new_acl()
{
  ymuint dsize = mScanner.cur_dsize();
  assert_cond( dsize % 6 == 0, __FILE__, __LINE__);
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
  assert_cond( dsize % 8 == 0, __FILE__, __LINE__);
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
