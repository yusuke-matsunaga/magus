#ifndef GDS_GDSPARSER_H
#define GDS_GDSPARSER_H

/// @file gds/GdsParser.h
/// @brief GdsParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"
#include "gds/GdsScanner.h"
#include "utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsParser GdsParser.h "gds/GdsParser.h"
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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief HEADER の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurData に格納される．
  /// mCurStruct を NULL に初期化する．
  bool
  read_header();

  /// @brief BGNSTR 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurStruct に格納される．
  /// mCurElement を NULL に初期化する．
  bool
  read_structure();

  /// @brief BOUNDARY 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_boundary();

  /// @brief PATH 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_path();

  /// @brief SREF 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_sref();

  /// @brief AREF 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_aref();

  /// @brief TEXT 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_text();

  /// @brief NODE 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_node();

  /// @brief BOX 以降の読み込み
  ///
  /// エラーが起きたら false を返す．
  /// 結果は mCurElement に格納される．
  bool
  read_box();

  /// @brief STRANS 以降の読み込み
  /// @param[out] strans 結果を格納する変数
  ///
  /// エラーが起きたら false を返す．
  bool
  read_strans(GdsStrans*& strans);

  /// @brief GdsElement (の派生要素)の追加
  void
  add_element(GdsElement* elem);

  /// @brief GdsProperty を作成し，property リストに追加する．
  /// @param[in] attr PROPATTR の値
  /// @param[in] value PROPVALUE の値
  void
  add_property(ymuint attr,
	       GdsString* value);

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

  /// @brief GdsFormat の作成
  GdsFormat*
  new_format();

  /// @brief GdsACL の作成
  GdsACL*
  new_acl();

  /// @brief COLROW の作成
  ymuint32
  new_colrow();

  /// @brief GdsDate の作成
  GdsDate*
  new_date();

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

  // 現在の GdsData
  GdsData* mCurData;

  // 現在の GdsStruct
  GdsStruct* mCurStruct;

  // 現在の GdsElement
  GdsElement* mCurElement;

  // 現在の GdsProperty の末尾
  GdsProperty* mCurProperty;

  // フォーマット番号
  ymuint8 mFormatType;

  // マスクのリスト
  vector<GdsString*> mMasks;
};

END_NAMESPACE_YM_GDS

#endif // GDS_GDSPARSER_H
