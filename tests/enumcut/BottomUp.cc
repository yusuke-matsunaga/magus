
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
BottomUp::operator()(const BdnMgr& network,
		     ymuint limit)
{
  all_init(network, limit);

  ymuint n = network.max_node_id();
  mCutListArray.clear();
  mCutListArray.resize(n);

  mTmpInputs = new ymuint32[limit];

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;

    node_init(node);

    list<vector<ymuint32> >& cut_list = mCutListArray[node->id()];
    cut_list.push_back(vector<ymuint32>(1, node->id()));

    found_cut(node);

    node_end(node);
  }

  vector<BdnNode*> node_list;
  network.sort(node_list);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;

    node_init(node);

    const BdnNode* node0 = node->fanin(0);
    const BdnNode* node1 = node->fanin(1);
    const list<vector<ymuint32> >& cut_list0 = mCutListArray[node0->id()];
    const list<vector<ymuint32> >& cut_list1 = mCutListArray[node1->id()];
    list<vector<ymuint32> >& cut_list = mCutListArray[node->id()];
    for (list<vector<ymuint32> >::const_iterator p = cut_list0.begin();
	 p != cut_list0.end(); ++ p) {
      const vector<ymuint32>& cut0 = *p;
      for (list<vector<ymuint32> >::const_iterator q = cut_list1.begin();
	   q != cut_list1.end(); ++ q) {
	const vector<ymuint32>& cut1 = *q;

	mInputNum = 0;
	vector<ymuint32>::const_iterator r1 = cut0.begin();
	vector<ymuint32>::const_iterator r2 = cut1.begin();
	while ( r1 != cut0.end() && r2 != cut1.end() ) {
	  ymuint id1 = *r1;
	  ymuint id2 = *r2;
	  if ( id1 < id2 ) {
	    if ( mInputNum >= limit ) {
	      goto overflow1;
	    }
	    mTmpInputs[mInputNum] = id1;
	    ++ mInputNum;
	    ++ r1;
	  }
	  else if ( id1 > id2 ) {
	    if ( mInputNum >= limit ) {
	      goto overflow1;
	    }
	    mTmpInputs[mInputNum] = id2;
	    ++ mInputNum;
	    ++ r2;
	  }
	  else {
	    if ( mInputNum >= limit ) {
	      goto overflow1;
	    }
	    mTmpInputs[mInputNum] = id1;
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
	{
	  vector<ymuint32> new_list(mInputNum);
	  for (ymuint i = 0; i < mInputNum; ++ i) {
	    new_list[i] = mTmpInputs[i];
	  }
	  cut_list.push_back(new_list);
	}

	found_cut(node, mInputNum, mTmpInputs);
      overflow1:
	;
      }
    }
    cut_list.push_back(vector<ymuint32>(1, node->id()));
    found_cut(node);

    node_end(node);
  }

  delete [] mTmpInputs;

  all_end(network, limit);

}

END_NAMESPACE_YM_NETWORKS
