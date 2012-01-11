
/// @file BottomUp.cc
/// @brief BottomUp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BottomUp.h"

#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

// @brief コンストラクタ
BottomUp::BottomUp()
{
}

// @brief デストラクタ
BottomUp::~BottomUp()
{
}

// @brief カット列挙を行う．
// @param[in] network 対象のネットワーク
// @param[in] limit カットサイズの制限
void
BottomUp::operator()(BdnMgr& network,
		     ymuint limit,
		     EnumCutOp* op)
{
  op->all_init(network, limit);

  ymuint n = network.max_node_id();

  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mMark1 = 0U;
    mNodeInfo[i].mMark2 = 0U;
  }

  mTmpInputs = new BdnNode*[limit];

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    BdnNode* node = *p;

    op->node_init(node);

    list<vector<BdnNode*> >& cut_list = mNodeInfo[node->id()].mCutList;
    cut_list.push_back(vector<BdnNode*>(1, node));

    op->found_cut(node, 0, NULL);

    op->node_end(node);
  }

  vector<BdnNode*> node_list;
  network.sort(node_list);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    BdnNode* node = *p;

    op->node_init(node);
    NodeInfo& node_info = mNodeInfo[node->id()];

    BdnNode* node0 = node->fanin(0);
    BdnNode* node1 = node->fanin(1);
    NodeInfo& node_info0 = mNodeInfo[node0->id()];
    NodeInfo& node_info1 = mNodeInfo[node1->id()];
    const list<vector<BdnNode*> >& cut_list0 = node_info0.mCutList;
    const list<vector<BdnNode*> >& cut_list1 = node_info1.mCutList;
    list<vector<BdnNode*> >& cut_list = node_info.mCutList;
    for (list<vector<BdnNode*> >::const_iterator p = cut_list0.begin();
	 p != cut_list0.end(); ++ p) {
      const vector<BdnNode*>& cut0 = *p;
      for (vector<BdnNode*>::const_iterator s = cut0.begin();
	   s != cut0.end(); ++ s) {
	ymuint id = (*s)->id();
	mNodeInfo[id].mMark1 = 2;
      }
      mark_cut1(node0);
      for (list<vector<BdnNode*> >::const_iterator q = cut_list1.begin();
	   q != cut_list1.end(); ++ q) {
	const vector<BdnNode*>& cut1 = *q;

	for (vector<BdnNode*>::const_iterator s = cut1.begin();
	     s != cut1.end(); ++ s) {
	  ymuint id = (*s)->id();
	  if ( mNodeInfo[id].mMark1 == 1 ) {
	    goto overflow1;
	  }
	}

	for (vector<BdnNode*>::const_iterator s = cut1.begin();
	     s != cut1.end(); ++ s) {
	  ymuint id = (*s)->id();
	  mNodeInfo[id].mMark2 = 2;
	}
	mark_cut2(node1);

	for (vector<BdnNode*>::const_iterator s = cut0.begin();
	     s != cut0.end(); ++ s) {
	  ymuint id = (*s)->id();
	  if ( mNodeInfo[id].mMark2 == 1 ) {
	    goto overflow1;
	  }
	}

	mInputNum = 0;
	{
	  vector<BdnNode*>::const_iterator r1 = cut0.begin();
	  vector<BdnNode*>::const_iterator r2 = cut1.begin();
	  while ( r1 != cut0.end() && r2 != cut1.end() ) {
	    BdnNode* node1 = *r1;
	    BdnNode* node2 = *r2;
	    ymuint id1 = node1->id();
	    ymuint id2 = node2->id();
	    if ( id1 < id2 ) {
	      if ( mInputNum >= limit ) {
		goto overflow1;
	      }
	      mTmpInputs[mInputNum] = node1;
	      ++ mInputNum;
	      ++ r1;
	    }
	    else if ( id1 > id2 ) {
	      if ( mInputNum >= limit ) {
		goto overflow1;
	      }
	      mTmpInputs[mInputNum] = node2;
	      ++ mInputNum;
	      ++ r2;
	    }
	    else {
	      if ( mInputNum >= limit ) {
		goto overflow1;
	      }
	      mTmpInputs[mInputNum] = node1;
	      ++ mInputNum;
	      ++ r1;
	      ++ r2;
	    }
	  }
	  for ( ; r1 != cut0.end(); ++ r1) {
	    if ( mInputNum >= limit ) {
	      goto overflow1;
	    }
	    mTmpInputs[mInputNum] = *r1;
	    ++ mInputNum;
	  }
	  for ( ; r2 != cut1.end(); ++ r2) {
	    if ( mInputNum >= limit ) {
	      goto overflow1;
	    }
	    mTmpInputs[mInputNum] = *r2;
	    ++ mInputNum;
	  }

	  vector<BdnNode*> new_list(mInputNum);
	  for (ymuint i = 0; i < mInputNum; ++ i) {
	    new_list[i] = mTmpInputs[i];
	  }
	  cut_list.push_back(new_list);
	}

	op->found_cut(node, mInputNum, mTmpInputs);
      overflow1:
	clear_cut2(node1);
      }
      clear_cut1(node0);
    }
    cut_list.push_back(vector<BdnNode*>(1, node));
    op->found_cut(node, 0, NULL);

    op->node_end(node);
  }

  delete [] mTmpInputs;

  op->all_end(network, limit);

}

void
BottomUp::mark_cut1(BdnNode* node)
{
  ymuint8& mark = mNodeInfo[node->id()].mMark1;

  if ( mark != 0 ) {
    return;
  }

  mark = 1;

  mark_cut1(node->fanin0());
  mark_cut1(node->fanin1());
}

void
BottomUp::clear_cut1(BdnNode* node)
{
  ymuint8& mark = mNodeInfo[node->id()].mMark1;

  if ( mark == 0 ) {
    return;
  }

  mark = 0;

  if ( node->is_logic() ) {
    clear_cut1(node->fanin0());
    clear_cut1(node->fanin1());
  }
}

void
BottomUp::mark_cut2(BdnNode* node)
{
  ymuint8& mark = mNodeInfo[node->id()].mMark2;

  if ( mark != 0 ) {
    return;
  }

  mark = 1;

  mark_cut2(node->fanin0());
  mark_cut2(node->fanin1());
}

void
BottomUp::clear_cut2(BdnNode* node)
{
  ymuint8& mark = mNodeInfo[node->id()].mMark2;

  if ( mark == 0 ) {
    return;
  }

  mark = 0;

  if ( node->is_logic() ) {
    clear_cut2(node->fanin0());
    clear_cut2(node->fanin1());
  }
}

END_NAMESPACE_YM_NETWORKS
