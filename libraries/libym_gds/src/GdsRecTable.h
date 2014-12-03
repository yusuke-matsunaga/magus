#ifndef GDSLIB_GDSRECTABLE_H
#define GDSLIB_GDSRECTABLE_H

/// @file GdsRecTable.h
/// @brief GdsRecTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsRecTable GdsRecTable.h "GdsRecTable.h"
/// @brief GDS-II の予約語の変換テーブル
//////////////////////////////////////////////////////////////////////
class GdsRecTable
{
private:
  // singleton パターンなので勝手に生成/破壊させない．

  // コンストラクタ
  GdsRecTable();

  // デストラクタ
  ~GdsRecTable();


public:

  // 唯一のオブジェクトを取り出す．
  static
  const GdsRecTable&
  obj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // レコード型からトークン値を得る．
  int
  rtype_token(GdsRtype rtype) const;

  // レコード型から文字列を得る．
  const char*
  rtype_string(GdsRtype rtype) const;

  // このレコード型に対応したデータ型を得る．
  GdsDtype
  dtype(GdsRtype rtype) const;

  // データ型から文字列を得る．
  const char*
  dtype_string(GdsDtype dtype) const;

  // レコード型から期待されるデータ数を得る．
  // 負数の場合にはその絶対値の倍数の可変個数の意味
  int
  data_num(GdsRtype rtype) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ使われる構造体
  //////////////////////////////////////////////////////////////////////
  struct cell
  {
    // レコード型
    GdsRtype mRtype;

    // トークン値
    int mToken;

    // 文字列(表示用)
    const char* mString;

    // 期待されるデータ型
    GdsDtype mDtype;

    // データ数
    // 負数の場合にはその絶対値の倍数の可変
    int mDataNum;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テーブル
  cell mTable[kGdsLast + 1];

};

END_NAMESPACE_YM_GDS

#endif
