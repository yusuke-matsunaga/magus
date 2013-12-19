
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
  for (ymuint i = 0; i < n; ++ i) {
    mMgr.new_var(VarId(i));
  }

  mNodeTemp.clear();
  mNodeTemp.resize(n);

  mpz_class nc_all = 0;

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    NodeTemp& nt = mNodeTemp[node->id()];

    Zdd cut = mMgr.make_base();
    cut.swap(VarId(node->id()));

    nt.mCut = cut;
    nt.mFpNodeList.push_back(node->id());

    mpz_class nc = cut.count();
    nc_all += nc;
    cout << "Node#" << node->id() << endl;
    cut.print_set(cout);
    cout << endl;
  }

  vector<const BdnNode*> node_list;
  network.sort(node_list);
  for (vector<const BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    NodeTemp& nt = mNodeTemp[node->id()];

    const BdnNode* node0 = node->fanin(0);
    const BdnNode* node1 = node->fanin(1);

    NodeTemp& nt0 = mNodeTemp[node0->id()];
    NodeTemp& nt1 = mNodeTemp[node1->id()];

    mNodeTemp[node->id()].mMark = 1;
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
    cut = mMgr.make_minimal(cut);

    VarVector sup_list;
    cut.support(sup_list);
    for (VarVector::iterator p = sup_list.begin();
	 p != sup_list.end(); ++ p) {
      VarId id = *p;
      nt.mFpNodeList.push_back(id.val());
    }

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
ZddTopDown::dfs(const BdnNode* node,
		const Zdd& cut)
{
  if ( mNodeTemp[node->id()].mMark == 0 ||
       !node->is_logic() ) {
    return cut;
  }
  mNodeTemp[node->id()].mMark = 0;

  const BdnNode* node0 = node->fanin0();
  const BdnNode* node1 = node->fanin1();

  // cut 中の node を node0 と node1 に置き換える．
  VarId vid(node->id());
  VarId vid0(node0->id());
  VarId vid1(node1->id());

  Zdd cut1 = cut.cofactor1(vid);

  Zdd tmp10 = cut1.cofactor0(vid0);
  Zdd tmp11 = cut1.cofactor1(vid0);
  Zdd tmp = tmp10 | tmp11;
  tmp.swap(vid0);

  Zdd tmp20 = tmp.cofactor0(vid1);
  Zdd tmp21 = tmp.cofactor1(vid1);
  Zdd tmp2 = tmp20 | tmp21;
  tmp2.swap(vid1);

  Zdd ans = cut | tmp2;

  ans = dfs(node0, ans);
  ans = dfs(node1, ans);

  return ans;
}

END_NAMESPACE_YM_NETWORKS
