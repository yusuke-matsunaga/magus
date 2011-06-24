#ifndef LIBYM_TECHMAP_CELLMAP_LATCHGROUP_H
#define LIBYM_TECHMAP_CELLMAP_LATCHGROUP_H

/// @file libym_techmap/cellmap/LatchGroup.h
/// @brief LatchGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroup.h"
#include "LatchPosArray.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class LatchGroup LatchGroup.h "LatchGroup.h"
/// @brief ラッチのグループを表すセル
//////////////////////////////////////////////////////////////////////
class LatchGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  LatchGroup();

  /// @brief デストラクタ
  ~LatchGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号の情報を返す．
  const LatchPosArray&
  pos_array() const;

  /// @brief セル数を返す．
  ymuint
  cell_num() const;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  const Cell*
  cell(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン番号などの情報
  LatchPosArray mPosArray;

  // セルのリスト
  vector<const Cell*> mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LatchGroup::LatchGroup()
{
}

// @brief デストラクタ
inline
LatchGroup::~LatchGroup()
{
}

// @brief ピンの割り当て情報を返す．
inline
const LatchPosArray&
LatchGroup::pos_array() const
{
  return mPosArray;
}

// @brief セル数を返す．
inline
ymuint
LatchGroup::cell_num() const
{
  return mCellList.size();
}

// @brief セルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
inline
const Cell*
LatchGroup::cell(ymuint pos) const
{
  return mCellList[pos];
}


END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_LATCHGROUP_H
