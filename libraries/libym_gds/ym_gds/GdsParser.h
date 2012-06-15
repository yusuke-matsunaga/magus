#ifndef YM_GDS_GDSPARSER_H
#define YM_GDS_GDSPARSER_H

/// @file ym_gds/GdsParser.h
/// @brief GdsParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsScanner.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_GDS_PARSER

// かなりかっこわるい & 危ない使い方．
union YYSTYPE;

END_NAMESPACE_YM_GDS_PARSER

BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsParser GdsParser.h "ym_gds/GdsParser.h"
/// @brief GDS-II のパーサークラス
//////////////////////////////////////////////////////////////////////
class GdsParser
{
public:

  /// @brief コンストラクタ
  GdsParser();

  /// @brief デストラクタ
  ~GdsParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  parse(const string& filename);


public:
  //////////////////////////////////////////////////////////////////////
  // gds_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief GdsAref の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] strname 構造名
  /// @param[in] strans STRANS の値
  /// @param[in] colrow 列と行の数
  /// @param[in] xy 座標
  GdsElement*
  new_aref(ymuint16 elflags,
	   ymint32 plex,
	   GdsString* strname,
	   GdsStrans* strans,
	   ymuint32 colrow,
	   GdsXY* xy);

  /// @brief GdsBoundary の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] xy XY の値
  GdsElement*
  new_boundary(ymuint16 elflags,
	       ymint32 plex,
	       ymint16 layer,
	       ymint16 datatype,
	       GdsXY* xy);

  /// @brief GdsBox の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] boxtype BOXTYPE の値
  /// @param[in] xy XY の値
  GdsElement*
  new_box(ymuint16 elflags,
	  ymint32 plex,
	  ymint16 layer,
	  ymint16 datatype,
	  GdsXY* xy);

  /// @brief GdsNode の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] xy XY の値
  GdsElement*
  new_node(ymuint16 elflags,
	   ymint32 plex,
	   ymint16 layer,
	   ymint16 nodetype,
	   GdsXY* xy);

  /// @brief GdsPath の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] pathtype PATYTYPE の値
  /// @param[in] width WIDTH の値
  /// @param[in] bgn_extn BGNEXTN の値
  /// @param[in] end_extn ENDEXTN の値
  /// @param[in] xy XY の値
  GdsElement*
  new_path(ymuint16 elflags,
	   ymint32 plex,
	   ymint16 layer,
	   ymint16 datatype,
	   ymint16 pathtype,
	   ymint32 width,
	   ymint32 bgn_extn,
	   ymint32 end_extn,
	   GdsXY* xy);

  /// @brief GdsSref の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] strname 構造名
  /// @param[in] strans STRANS の値
  /// @param[in] xy 座標
  GdsElement*
  new_sref(ymuint16 elflags,
	   ymint32 plex,
	   GdsString* strname,
	   GdsStrans* strans,
	   GdsXY* xy);

  /// @brief GdsText の作成
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] texttype TEXTTYPE の値
  /// @param[in] presentation PRESENTATION の値
  /// @param[in] pathtype PATHTYPE の値
  /// @param[in] width WIDTH の値
  /// @param[in] strans STRANS の値
  /// @param[in] xy XY座標
  /// @param[in] body 本体の文字列
  GdsElement*
  new_text(ymuint16 elflags,
	   ymint32 plex,
	   ymint16 layer,
	   ymint16 texttype,
	   ymuint16 presentation,
	   ymint16 pathtype,
	   ymint32 width,
	   GdsStrans* strans,
	   GdsXY* xy,
	   GdsString* body);

  /// @brief GdsStrans の作成
  GdsStrans*
  new_strans(ymuint flags,
	     double mag,
	     double angle);

  /// @brief property リストのクリア
  void
  clear_property();

  /// @brief GdsProperty を作成し，property リストに追加する．
  /// @param[in] attr PROPATTR の値
  /// @param[in] value PROPVALUE の値
  void
  add_property(ymuint attr,
	       GdsString* value);

  /// @brief property リストを GdsElement にセットする．
  void
  set_property(GdsElement* elem);

  /// @brief yylex() の実装
  int
  yylex(nsParser::YYSTYPE& lval);

  /// @brief yyerror() の実装
  int
  yyerror(const char* s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief GdsACL の作成
  GdsACL*
  new_acl();

  /// @brief COLROW の作成
  ymuint32
  new_colrow();

  /// @brief GdsDate の作成
  GdsDate*
  new_date();

  /// @brief GdsFormat の作成
  GdsFormat*
  new_format();

  /// @brief GdsString の作成
  GdsString*
  new_string();

  /// @brief GdsUnits の作成
  GdsUnits*
  new_units();

  /// @brief GdsXY の作成
  GdsXY*
  new_xy();

  /// @brief int2 の作成
  ymint16
  new_int2();

  /// @brief int4 の作成
  ymint32
  new_int4();

  /// @brief real の作成
  double
  new_real();

  /// @brief BitArray の作成
  ymuint16
  new_bitarray();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 領域確保用のオブジェクト
  SimpleAlloc mAlloc;

  // 字句解析器
  GdsScanner mScanner;

  // property のリスト
  vector<GdsProperty*> mPropList;

};

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSPARSER_H
