
/// @file BNode.cc
/// @brief BNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetwork.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
// ノードを表すクラス BNode
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 節点のタイプは中間節点用が設定される．
// ファンインはNULLにしておく．
BNode::BNode() :
  mFlags(0),
  mFaninEdgeArray(NULL),
  mParent(NULL)
{
  flags_set_type(kLO);
}

// デストラクタ
BNode::~BNode()
{
  assert_cond( fanout_num() == 0, __FILE__, __LINE__);
  assert_cond( fanin_num() == 0, __FILE__, __LINE__);
}

// ノードタイプをセットする．
// ついでに付随するいくつかの設定を行う．
void
BNode::set_type(tType type)
{
  flags_set_type(type);

  switch (type) {
  case kPI:
    break;
  case kPO:
    mFunc = Expr::make_posiliteral(VarId(0));
    break;
  case kLO:
    break;
  case kFF:
    mFunc = Expr::make_zero();
    break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

// 引数の節点が何番目のファンインか調べる．
// ファンインではない場合には-1を返す．
int
BNode::fanin_pos(BNode* node) const
{
  int i;
  for (i = fanin_num(); -- i >= 0; ) {
    if ( mFaninEdgeArray[i].from() == node ) break;
  }
  // 範囲外の場合にも具合良く-1になっている．
  return i;
}

// SOP形式のリテラル数の見積りを返す．
ymuint
BNode::sop_litnum() const
{
  return mFunc.sop_litnum();
}

// valueを計算する．
int
BNode::value() const
{
  // まずリテラル数を求める．
  int lit_num = litnum();

  // つぎにこのノードの出現回数を数える．
  int c = 0;
  bool feed_to_outputs = false;
  for (BNodeFoList::const_iterator p = fanouts_begin();
       p != fanouts_end(); ++ p) {
    BNodeEdge* edge = *p;
    BNode* onode = edge->to();
    if ( onode->is_output() ) {
      feed_to_outputs = true;
    }
    else {
      c += onode->mFunc.litnum(VarId(edge->pos()));
    }
  }

  if ( feed_to_outputs ) {
    // 外部出力にファンアウトしているときは削除できない．
    return c * (lit_num - 1);
  }
  else {
    // Value を返す．
    return (c - 1) * (lit_num - 1) - 1;
  }
}

END_NAMESPACE_YM_NETWORKS_BNET
