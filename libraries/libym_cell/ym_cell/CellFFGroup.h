#ifndef YM_CELL_CELLFFGROUP_H
#define YM_CELL_CELLFFGROUP_H

/// @file ym_cell/CellFFGroup.h
/// @brief CellFFGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"
#include "ym_cell/CellFFPosArray.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellFFGroup CellFFGroup.h "ym_cell/CellFFGroup.h"
/// @brief FF のグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CellFFGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellFFGroup();

  /// @brief デストラクタ
  ~CellFFGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンの割り当て情報を返す．
  const CellFFPosArray&
  pos_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン番号の情報
  CellFFPosArray mPosArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CellFFGroup::CellFFGroup()
{
}

// @brief デストラクタ
inline
CellFFGroup::~CellFFGroup()
{
}

// @brief ピンの割り当て情報を返す．
inline
const CellFFPosArray&
CellFFGroup::pos_array() const
{
  return mPosArray;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLFFGROUP_H
