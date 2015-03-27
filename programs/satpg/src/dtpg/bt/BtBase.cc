
/// @file BtBase.cc
/// @brief BtBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtBase::BtBase()
{
}

// @brief デストラクタ
BtBase::~BtBase()
{
}

// @brief ノードID番号の最大値を設定する．
/// @param[in] max_id ID番号の最大値
//
// このクラスの実装ではなにもしない．
void
BtBase::set_max_id(ymuint max_id)
{
}

// @brief 入力ノードの値を記録する．
// @param[in] node 対象の外部入力ノード
// @param[in] model SAT の割り当て結果
// @param[out] assign_list 値の割当リスト
void
BtBase::record_value(TpgNode* node,
		     const vector<Bool3>& model,
		     NodeValList& assign_list)
{
  Bool3 v = node_gval(node, model);
  if ( v == kB3False ) {
    assign_list.add(node, false);
  }
  else if ( v == kB3True ) {
    assign_list.add(node, true);
  }
}

END_NAMESPACE_YM_SATPG
