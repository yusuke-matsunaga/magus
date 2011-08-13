#ifndef YM_CELL_CELLGROUP_H
#define YM_CELL_CELLGROUP_H

/// @file ym_cell/CellGroup.h
/// @brief CellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellGroup CellGroup.h "ym_cell/CellGroup.h"
/// @brief 論理的に等価なセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CellGroup
{
  friend class CellMgr;

protected:

  /// @brief コンストラクタ
  CellGroup();

  /// @brief デストラクタ
  ~CellGroup();


public:

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

  // セル数
  ymuint32 mCellNum;

  // セルのポインタの配列
  const Cell** mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CellGroup::CellGroup() :
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
inline
CellGroup::~CellGroup()
{
  // mCellList は CellMgr が管理している．
}

// @brief セル数を返す．
inline
ymuint
CellGroup::cell_num() const
{
  return mCellNum;
}

// @brief セルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
inline
const Cell*
CellGroup::cell(ymuint pos) const
{
  return mCellList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLGROUP_H
