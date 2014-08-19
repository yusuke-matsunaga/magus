
/// @file CountOp.cc
/// @brief CountOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CountOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス CountOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
CountOp::CountOp(BddMgrImpl* mgr) :
  BddOp(mgr)
{
}

// @brief デストラクタ
CountOp::~CountOp()
{
}

// @brief 演算を行う関数
// @param[in] edge_list 根の枝のリスト
// @return edge_list の根からたどれる BDD のノード数を返す．
ymuint64
CountOp::apply(const vector<BddEdge>& edge_list)
{
  mNum = 0;
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    count_sub(e);
  }
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    clear_mark(e);
  }
  return mNum;
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
CountOp::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数
void
CountOp::count_sub(BddEdge e)
{
  // ちょっと分かりにくいけど node->edge1() への再帰を
  // for-loop に変換している．
  for ( ; ; ) {
    BddNode* node = e.get_node();
    if ( node == 0 || node->pmark() ) {
      return;
    }
    ++ mNum;
    node->pmark(1);
    count_sub(node->edge0());
    e = node->edge1();
  }
}

END_NAMESPACE_YM_BDD
