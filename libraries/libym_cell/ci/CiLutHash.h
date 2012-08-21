#ifndef CILUTHASH_H
#define CILUTHASH_H

/// @file CiLutHash.h
/// @brief CiLutHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

class CiLutTemplate;

//////////////////////////////////////////////////////////////////////
/// @class CiLutHash CiLutHash.h "CiLutHash.h"
/// @brief 名前をキーにした CiCell のハッシュ表
//////////////////////////////////////////////////////////////////////
class CiLutHash
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  CiLutHash(Alloc& alloc);

  /// @brief デストラクタ
  ~CiLutHash();


public:

  /// @brief テンプレートを追加する．
  /// @param[in] templ 追加するテンプレート
  void
  add(CiLutTemplate* templ);

  /// @brief テンプレートを取り出す．
  /// @param[in] name 名前
  /// @return name という名前のテンプレートを返す．
  /// @note なければ NULL を返す．
  CiLutTemplate*
  get(ShString name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  /// @param[in] req_size 要求するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

  // テーブルサイズ
  ymuint32 mSize;

  // ハッシュ表
  CiLutTemplate** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_CELL

#endif // CILUTHASH_H
