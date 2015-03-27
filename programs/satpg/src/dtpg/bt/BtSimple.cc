
/// @file BtSimple.cc
/// @brief BtSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtSimple.h"


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
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
// @param[out] assign_list 値の割当リスト
void
BtSimple::operator()(TpgNode* fnode,
		     const vector<Bool3>& model,
		     const vector<TpgNode*>& input_list,
		     const vector<TpgNode*>& output_list,
		     AssignList& assign_list)
{
  assign_list.clear();
  for (vector<TpgNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    TpgNode* node = *p;
    record_value(node, model, assign_list);
  }
}

END_NAMESPACE_YM_SATPG
