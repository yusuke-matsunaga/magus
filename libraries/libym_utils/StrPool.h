#ifndef STRPOOL_H
#define STRPOOL_H

/// @file StrPool.h
/// @brief StrPool のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StrPool StrPool.h "StrPool.h"
/// @ingroup YmUtils
/// @brief 文字列を共有するためのプール
///
/// メンバ関数 reg() で文字列を登録する．
/// 文字列へのポインタが返ってくるが，これはこのオブジェクト内で共有化
/// されている．
/// @note このクラスではいったん登録した文字列を削除する方法はない．
//////////////////////////////////////////////////////////////////////
class StrPool
{
public:

  /// @brief コンストラクタ
  StrPool();

  /// @brief デストラクタ
  /// @note このオブジェクトが管理しているすべてのページが解放される．
  ~StrPool();


public:

  /// @brief 文字列を登録する．
  /// @param[in] str 入力となる文字列
  /// @return カノニカライズされた文字列を返す．
  const char*
  reg(const char* str);

  /// @brief 確保した文字列領域の総量を得る．
  /// @return 確保した文字列領域の総量を得る．
  /// @note デバッグ/解析用 -- 通常は使わない．
  ymuint
  accum_alloc_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ関数
  /// @param[in] str ハッシュ対象の文字列
  /// @return str のハッシュ値
  static
  ymuint32
  hash_func(const char* str);

  /// @brief テーブルを確保して初期化する．
  /// @param[in] new_size 新しいテーブルサイズ
  void
  alloc_table(ymuint32 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表の中で使われる要素
  struct
  Cell
  {
    // 次のセルを指すリンクポインタ
    Cell* mLink;

    // 文字数 (末尾の \0 を含む)
    ymuint32 mSize;

    // 文字列領域の先頭を指すダミー
    // 実際には必要なサイズの領域を確保する．
    char mStr[1];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべての文字列を蓄えておくハッシュ表
  Cell** mTable;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表の実効サイズ
  ymuint32 mHashMask;

  // 登録されている要素数
  ymuint32 mNum;

  // 次に拡張する基準
  ymuint32 mExpandLimit;

  // Cell を確保するためのアロケータ
  SimpleAlloc mCellAlloc;

};

END_NAMESPACE_YM

#endif // STRPOOL_H
