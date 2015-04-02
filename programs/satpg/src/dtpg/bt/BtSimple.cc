
/// @file BtSimple.cc
/// @brief BtSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtSimple.h"
#include "NodeSet.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'Simple' タイプの生成を行なう．
BackTracer*
new_BtSimple()
{
  return new BtSimple();
}


//////////////////////////////////////////////////////////////////////
// クラス BtSimple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtSimple::BtSimple()
{
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] node_set 故障に関係するノード集合
// @param[in] val_map ノードの値の割当を保持するクラス
// @param[out] assign_list 値の割当リスト
void
BtSimple::operator()(TpgNode* fnode,
		     const NodeSet& node_set,
		     const ValMap& val_map,
		     NodeValList& assign_list)
{
  assign_list.clear();
  for (vector<TpgNode*>::const_iterator p = node_set.input_list().begin();
       p != node_set.input_list().end(); ++ p) {
    TpgNode* node = *p;
    record_value(node, val_map, assign_list);
  }
}

END_NAMESPACE_YM_SATPG
