#ifndef YM_CELL_CELLFUNCGROUP_H
#define YM_CELL_CELLFUNCGROUP_H

/// @file ym_cell/CellFuncGroup.h
/// @brief CellFuncGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellFuncGroup CellFuncGroup.h "ym_cell/CellFuncGroup.h"
/// @brief 論理的に等価なセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CellFuncGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellFuncGroup();

  /// @brief デストラクタ
  ~CellFuncGroup();


public:

  /// @brief 代表関数に対する変換マップを返す．
  const NpnMap&
  npn_map() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表関数に対する変換マップ
  NpnMap mNpnMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CellFuncGroup::CellFuncGroup()
{
}

// @brief デストラクタ
inline
CellFuncGroup::~CellFuncGroup()
{
}

// @brief 代表関数に対する変換マップを返す．
inline
const NpnMap&
CellFuncGroup::npn_map() const
{
  return mNpnMap;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLFUNCGROUP_H
