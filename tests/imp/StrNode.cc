
/// @file StrNode.cc
/// @brief StrNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス StrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node0 ファンイン0のノード
// @param[in] inv0 ファンイン0の極性
// @param[in] node1 ファンイン1のノード
// @param[in] inv1 ファンイン1の極性
StrNode::StrNode(ymuint id,
		 StrNode* node0,
		 bool inv0,
		 StrNode* node1,
		 bool inv1)
{
  mId = id;
  if ( node0 != NULL ) {
    mFanins[0].set(node0, inv0, this, 0);
    node0->mFanouts.push_back(&mFanins[0]);
  }
  if ( node1 != NULL ) {
    mFanins[1].set(node1, inv1, this, 1);
    node1->mFanouts.push_back(&mFanins[1]);
  }
}

// @brief デストラクタ
StrNode::~StrNode()
{
}

// @brief 外部入力のときに true を返す．
bool
StrNode::is_input() const
{
  return false;
}

// @brief AND タイプのときに true を返す．
bool
StrNode::is_and() const
{
  return false;
}

// @brief XOR タイプのときに true を返す．
bool
StrNode::is_xor() const
{
  return false;
}

#if 0
// @brief ファンアウト先に0を伝搬する．
bool
StrNode::fwd_prop0()
{
  mLearnedList.push_back(ImpCell(id(), 0));
  for (vector<StrEdge*>::iterator p = mFanouts.begin();
       p != mFanouts.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1();
      }
      else {
	stat = node->fwd1_imp1();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0();
      }
      else {
	stat = node->fwd1_imp0();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  return true;
}

// @brief ファンアウト先に1を伝搬する．
bool
StrNode::fwd_prop1()
{
  mLearnedList.push_back(ImpCell(id(), 1));
  for (vector<StrEdge*>::iterator p = mFanouts.begin();
       p != mFanouts.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0();
      }
      else {
	stat = node->fwd1_imp0();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1();
      }
      else {
	stat = node->fwd1_imp1();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  return true;
}

// @brief ファンイン0に0を伝搬する．
bool
StrNode::fanin0_prop0()
{
#if 0
  cout << "fanin0_prop0(" << id() << ")" << endl;
#endif
  StrEdge& e = mFanins[0];
  StrNode* node = e.src_node();
  if ( e.src_inv() ) {
    return node->bwd_prop1(this);
  }
  else {
    return node->bwd_prop0(this);
  }
}

// @brief ファンイン0に1を伝搬する．
bool
StrNode::fanin0_prop1()
{
#if 0
  cout << "fanin0_prop1(" << id() << ")" << endl;
#endif
  StrEdge& e = mFanins[0];
  StrNode* node = e.src_node();
  if ( e.src_inv() ) {
    return node->bwd_prop0(this);
  }
  else {
    return node->bwd_prop1(this);
  }
}

// @brief ファンイン1に0を伝搬する．
bool
StrNode::fanin1_prop0()
{
#if 0
  cout << "fanin1_prop0(" << id() << ")" << endl;
#endif
  StrEdge& e = mFanins[1];
  StrNode* node = e.src_node();
  if ( e.src_inv() ) {
    return node->bwd_prop1(this);
  }
  else {
    return node->bwd_prop0(this);
  }
}

// @brief ファンイン1に1を伝搬する．
bool
StrNode::fanin1_prop1()
{
#if 0
  cout << "fanin1_prop1(" << id() << ")" << endl;
#endif
  StrEdge& e = mFanins[1];
  StrNode* node = e.src_node();
  if ( e.src_inv() ) {
    return node->bwd_prop0(this);
  }
  else {
    return node->bwd_prop1(this);
  }
}

// @brief 後方含意を行う．
// @param[in] from_node 含意元のノード
bool
StrNode::bwd_prop0(StrNode* from_node)
{
#if 0
  cout << "bwd_prop0(" << id();
  if ( from_node ) {
    cout << " <- " << from_node->id();
  }
  cout << ")" << endl;
#endif
  for (vector<StrEdge*>::iterator p = mFanouts.begin();
       p != mFanouts.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    if ( node == from_node ) continue;
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1();
      }
      else {
	stat = node->fwd1_imp1();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0();
      }
      else {
	stat = node->fwd1_imp0();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  if ( val() == kB3X ) {
    mLearnedList.push_back(ImpCell(id(), 0));
  }
  return bwd_imp0();
}

// @brief 後方含意を行う．
// @param[in] from_node 含意元のノード
bool
StrNode::bwd_prop1(StrNode* from_node)
{
#if 0
  cout << "bwd_prop1(" << id();
  if ( from_node ) {
    cout << " <- " << from_node->id();
  }
  cout << ")" << endl;
#endif
  for (vector<StrEdge*>::iterator p = mFanouts.begin();
       p != mFanouts.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    if ( node == from_node ) continue;
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0();
      }
      else {
	stat = node->fwd1_imp0();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1();
      }
      else {
	stat = node->fwd1_imp1();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  if ( val() == kB3X ) {
    mLearnedList.push_back(ImpCell(id(), 1));
  }
  return bwd_imp1();
}
#endif

END_NAMESPACE_YM_NETWORKS
