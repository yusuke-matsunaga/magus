
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

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] val_list 求まったテストパタンを格納する配列
void
BtSimple::operator()(TpgNode* fnode,
		     const vector<TpgNode*>& output_list,
		     const vector<TpgNode*>& input_list,
		     vector<ymuint32>& val_list)
{
  for (vector<TpgNode*>::const_iterator p = inputList.begin();
       p != inputList.end(); ++ p) {
    TpgNode* node = *p;
    record_value(node);
  }
}

END_NAMESPACE_YM_SATPG
