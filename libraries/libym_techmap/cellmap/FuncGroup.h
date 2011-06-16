#ifndef LIBYM_TECHMAP_CELLMAP_FUNCGROUP_H
#define LIBYM_TECHMAP_CELLMAP_FUNCGROUP_H

/// @file libym_techmap/cellmap/FuncGroup.h
/// @brief FuncGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroup.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class FuncGroup FuncGroup.h "FuncGroup.h"
/// @brief 論理的に等価なセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class FuncGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  FuncGroup();

  /// @brief デストラクタ
  ~FuncGroup();


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
FuncGroup::FuncGroup()
{
}

// @brief デストラクタ
inline
FuncGroup::~FuncGroup()
{
}

// @brief 代表関数に対する変換マップを返す．
inline
const NpnMap&
FuncGroup::npn_map() const
{
  return mNpnMap;
}

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_FUNCGROUP_H
