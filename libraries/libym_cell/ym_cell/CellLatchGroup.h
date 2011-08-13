#ifndef YM_CELL_CELLLATCHGROUP_H
#define YM_CELL_CELLLATCHGROUP_H

/// @file ym_cell/CellLatchGroup.h
/// @brief CellLatchGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"
#include "ym_cell/CellLatchPosArray.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class LatchGroup LatchGroup.h "ym_cell/LatchGroup.h"
/// @brief ラッチのグループを表すセル
//////////////////////////////////////////////////////////////////////
class CellLatchGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellLatchGroup();

  /// @brief デストラクタ
  ~CellLatchGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号の情報を返す．
  const CellLatchPosArray&
  pos_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン番号などの情報
  CellLatchPosArray mPosArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CellLatchGroup::CellLatchGroup()
{
}

// @brief デストラクタ
inline
CellLatchGroup::~CellLatchGroup()
{
}

// @brief ピンの割り当て情報を返す．
inline
const CellLatchPosArray&
CellLatchGroup::pos_array() const
{
  return mPosArray;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLATCHGROUP_H
