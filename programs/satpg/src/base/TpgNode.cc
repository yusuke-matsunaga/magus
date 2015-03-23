
/// @file TpgNode.cc
/// @brief TpgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"
#include "TpgMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgNode
//////////////////////////////////////////////////////////////////////

// @brief TgNode のファンインに対応するノードを返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
TpgNode*
TpgNode::input_map(ymuint pos) const
{
  return mInputMap->input(pos);
}

// @brief TgNode のファンインに対応するノードのファンイン番号を返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
ymuint
TpgNode::ipos_map(ymuint pos) const
{
  return mInputMap->input_pos(pos);
}

END_NAMESPACE_YM_SATPG
