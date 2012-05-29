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

  /// @brief GdsColRow の作成
  GdsColRow*
  new_colrow();

  /// @brief GdsFormat の作成
  GdsFormat*
  new_format();

  /// @brief GdsDate の作成
  GdsDate*
  new_date();

  /// @brief GdsUnits の作成
  GdsUnits*
  new_units();

  /// @brief GdsString の作成
  GdsString*
  new_string();

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

};

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSPARSER_H
