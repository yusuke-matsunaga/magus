
/// @file LsimBdd.cc
/// @brief LsimBdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimBdd.h"
#include "networks/BdnNode.h"
#include "networks/BdnPort.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimBdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimBdd::LsimBdd() :
  mBddMgr("bmc", "Bdd Manager"),
  mOutputList(mBddMgr)
{
}

// @brief デストラクタ
LsimBdd::~LsimBdd()
{
}

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimBdd::set_network(const BdnMgr& bdn,
		     const unordered_map<string, ymuint>& order_map)
{
  ymuint n = bdn.max_node_id();
  vector<Bdd> bddmap(n);

  const BdnNodeList& input_list = bdn.input_list();
  if ( order_map.empty() ) {
    ymuint id = 0;
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      Bdd bdd = mBddMgr.make_posiliteral(VarId(id));
      ++ id;
      bddmap[node->id()] = bdd;
    }
  }
  else {
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      string name = node->port()->name();
      unordered_map<string, ymuint>::const_iterator q = order_map.find(name);
      if ( q == order_map.end() ) {
	cerr << "No map for " << name << endl;
	abort();
      }
      ymuint id = q->second;
      Bdd bdd = mBddMgr.make_posiliteral(VarId(id));
      bddmap[node->id()] = bdd;
    }
  }

  vector<const BdnNode*> node_list;
  bdn.sort(node_list);
  for (vector<const BdnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* fanin0 = node->fanin0();
    Bdd bdd0 = bddmap[fanin0->id()];
    if ( node->fanin0_inv() ) {
      bdd0 = ~bdd0;
    }
    const BdnNode* fanin1 = node->fanin1();
    Bdd bdd1 = bddmap[fanin1->id()];
    if ( node->fanin1_inv() ) {
      bdd1 = ~bdd1;
    }
#if 0
    cout << " " << bdd0.size() << " x " << bdd1.size();
    cout.flush();
#endif
    if ( node->is_and() ) {
      Bdd bdd = bdd0 & bdd1;
      bddmap[node->id()] = bdd;
    }
    else if ( node->is_xor() ) {
      Bdd bdd = bdd0 ^ bdd1;
      bddmap[node->id()] = bdd;
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
#if 0
    cout << " = " << bddmap[node->id()].size();
#endif
  }

  mBddMgr.disable_gc();

  const BdnNodeList& output_list = bdn.output_list();
  mOutputList.clear();
  mOutputList.reserve(output_list.size());
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BdnNode* node = *p;
    BdnNode* node0 = node->output_fanin();
    Bdd bdd;
    if ( node0 ) {
      bdd = bddmap[node0->id()];
    }
    else {
      bdd = mBddMgr.make_zero();
    }
    if ( node->output_fanin_inv() ) {
      bdd = ~bdd;
    }
    mOutputList.push_back(bdd);
  }

  cout << "BDD size: " << mOutputList.node_count() << endl;
}


BEGIN_NONAMESPACE

ymuint64
eval_bdd(Bdd bdd0,
	 const vector<ymuint64>& iv)
{
  ymuint val = 0U;
  for (ymuint b = 0; b < 64; ++ b) {
    Bdd bdd = bdd0;
    ymuint64 bit = 1UL << b;
    for ( ; ; ) {
      if ( bdd.is_one() ) {
	val |= bit;
	break;
      }
      else if ( bdd.is_zero() ) {
	break;
      }
      Bdd bdd0;
      Bdd bdd1;
      VarId id = bdd.root_decomp(bdd0, bdd1);
      ymuint64 ival = iv[id.val()];
      if ( ival & bit ) {
	bdd = bdd1;
      }
      else {
	bdd = bdd0;
      }
    }
  }
  return val;
}

END_NONAMESPACE


// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimBdd::eval(const vector<ymuint64>& iv,
	      vector<ymuint64>& ov)
{
  ymuint no = ov.size();
  for (ymuint i = 0; i < no; ++ i) {
    Bdd bdd = mOutputList[i];
    ov[i] = eval_bdd(bdd, iv);
  }
}

END_NAMESPACE_YM
