#ifndef CICELLHASH_H
#define CICELLHASH_H

/// @file CiCellHash.h
/// @brief CiCellHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/cell_nsdef.h"
#include "utils/ShString.h"
#include "utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

class CiCell;

//////////////////////////////////////////////////////////////////////
/// @class CiCellHash CiCellHash.h "CiCellHash.h"
/// @brief 名前をキーにした CiCell のハッシュ表
//////////////////////////////////////////////////////////////////////
class CiCellHash
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  CiCellHash(Alloc& alloc);

  /// @brief デストラクタ
  ~CiCellHash();


public:

  /// @brief セルを追加する．
  /// @param[in] cell 追加するセル
  void
  add(CiCell* cell);

  /// @brief セルを取り出す．
  /// @param[in] name 名前
  /// @return name という名前のセルを返す．
  /// @note なければ NULL を返す．
  CiCell*
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
  CiCell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_CELL

#endif // CICELLHASH_H
