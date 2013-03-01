
/// @file BtSimple.cc
/// @brief BtSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "BtSimple.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtSimple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
BtSimple::BtSimple(ymuint max_id) :
  BtBase(max_id)
{
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
void
BtSimple::operator()(TpgNode* fnode,
		     const vector<TpgNode*>& input_list,
		     const vector<TpgNode*>& output_list)
{
  clear_val_list();

  for (vector<TpgNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    TpgNode* node = *p;
    record_value(node);
  }
}

END_NAMESPACE_YM_SATPG
