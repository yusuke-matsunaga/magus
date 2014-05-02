
/// @file TpgMap.cc
/// @brief TpgMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgPrimMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node 根のノード
TpgPrimMap::TpgPrimMap(TpgNode* node) :
  mNode(node)
{
}

// @brief デストラクタ
TpgPrimMap::~TpgPrimMap()
{
}

// @brief 入力に対応するノードを得る．
// @param[in] ipos 入力の位置番号
TpgNode*
TpgPrimMap::input(ymuint ipos) const
{
  return mNode;
}

// @brief 入力に対応するノードのファンイン番号を得る．
// @param[in] ipos 入力の位置番号
ymuint
TpgPrimMap::input_pos(ymuint ipos) const
{
  return ipos;
}


//////////////////////////////////////////////////////////////////////
// クラス TpgCplxMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TpgCplxMap::TpgCplxMap()
{
}

// @brief デストラクタ
TpgCplxMap::~TpgCplxMap()
{
}

// @brief 入力に対応するノードを得る．
// @param[in] ipos 入力の位置番号
TpgNode*
TpgCplxMap::input(ymuint ipos) const
{
  return mInputNodeArray[ipos];
}

// @brief 入力に対応するノードのファンイン番号を得る．
// @param[in] ipos 入力の位置番号
ymuint
TpgCplxMap::input_pos(ymuint ipos) const
{
  return mInputPosArray[ipos];
}

END_NAMESPACE_YM_SATPG
