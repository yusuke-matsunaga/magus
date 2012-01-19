
/// @file ZddTopDown.cc
/// @brief ZddTopDown の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddTopDown.h"

#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

// @brief コンストラクタ
// @param[in] mgr
ZddTopDown::ZddTopDown(ZddMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
ZddTopDown::~ZddTopDown()
{
}

// @brief カット列挙を行う．
// @param[in] network 対象のネットワーク
// @param[in] limit カットサイズの制限
void
ZddTopDown::operator()(BdnMgr& network,
		       ymuint limit)
{
  ymuint n = network.max_node_id();

  mNodeTemp.clear();
  mNodeTemp.resize(n);

  mpz_class nc_all = 0;

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    BdnNode* node = *p;
    NodeTemp& nt = mNodeTemp[node->id()];

    Zdd cut = mMgr.make_base();
    cut.swap(VarId(node->id()));

    nt.mCut = cut;
    nt.mFpNodeList.push_back(node);

    mpz_class nc = cut.count();
    nc_all += nc;
    cout << "Node#" << node->id() << endl;
    cut.print_set(cout);
    cout << endl;
  }

  vector<BdnNode*> node_list;
  network.sort(node_list);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    BdnNode* node = *p;
    NodeTemp& nt = mNodeTemp[node->id()];

    BdnNode* node0 = node->fanin(0);
    BdnNode* node1 = node->fanin(1);

    NodeTemp& nt0 = mNodeTemp[node0->id()];
    NodeTemp& nt1 = mNodeTemp[node1->id()];

    for (vector<ymuint>::iterator p = nt0.mFpNodeList.begin();
	 p != nt0.mFpNodeList.end(); ++ p) {
      ymuint id = *p;
      mNodeTemp[id].mMark = 1;
    }
    for (vector<ymuint>::iterator p = nt1.mFpNodeList.begin();
	 p != nt1.mFpNodeList.end(); ++ p) {
      ymuint id = *p;
      mNodeTemp[id].mMark = 1;
    }

    Zdd cut = mMgr.make_base();
    cut.swap(VarId(node->id()));
    cut = dfs(node, cut);
    cut = mMgr.n_element(cut, limit);
    cut = mMgr.minimum_set(cut);

    nt.mCut = cut;
    mpz_class nc = cut.count();
    nc_all += nc;
    cout << "Node#" << node->id() << endl;
    cut.print_set(cout);
    cout << nc << " cuts, "
	 << cut.node_count() << " nodes" << endl;
    cout << endl;
  }
  cout << "Total " << nc_all << " cuts" << endl;
}

Zdd
ZddTopDown::dfs(Zdd
END_NAMESPACE_YM_NETWORKS
