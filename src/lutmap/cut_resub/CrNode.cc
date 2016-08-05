﻿
/// @file lutmap/CrNode.cc
/// @brief CrNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "CrNode.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス CrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CrNode::CrNode() :
  mNode(nullptr),
  mCurCut(nullptr),
  mFlags(0U),
  mCurGain(0),
  mHeapIndex(-1),
  mLevel(0),
  mReqLevel(0)
{
}

// @brief デストラクタ
CrNode::~CrNode()
{
}

// @brief 対応する SbjNode を設定する．
void
CrNode::set_sbjnode(const SbjNode* node)
{
  mNode = node;
  if ( node->is_input() ) {
    mFlags |= (1U << kInputShift);
  }
  if ( node->pomark() ) {
    mFlags |= (1U << kOutputShift);
  }
}

// @brief ファンアウトを追加する．
bool
CrNode::add_fanout(CrNode* add_node)
{
  for (vector<CrNode*>::iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    CrNode* fo = *p;
    if ( fo == add_node ) {
      return false;
    }
  }
  mFanoutList.push_back(add_node);
  return true;
}

// @brief ファンアウトリストから削除する．
void
CrNode::delete_fanout(CrNode* del_node)
{
  ymuint n = mFanoutList.size();
  ymuint k = 0;
  for (ymuint i = 0; i < n; ++ i) {
    CrNode* node1 = mFanoutList[i];
    if ( node1 != del_node ) {
      mFanoutList[k] = node1;
      ++ k;
    }
  }
  if ( k < n ) {
    mFanoutList.erase(mFanoutList.begin() + k, mFanoutList.end());
  }
}

// 比較関数
bool
compare(CrNode* node1,
	CrNode* node2)
{
  int diff = node1->gain() - node2->gain();
  if ( diff > 0 ) {
    return true;
  }
  if ( diff < 0 ) {
    return false;
  }
  // この比較に意味はないが結果を決定的にするため
  return node1->sbjnode()->id() < node2->sbjnode()->id();
}

END_NAMESPACE_YM_LUTMAP
