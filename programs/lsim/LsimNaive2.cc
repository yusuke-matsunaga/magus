
/// @file LsimNaive2.cc
/// @brief LsimNaive2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimNaive2.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

void
xoring(BdnMgr& bdnmgr)
{
  vector<const BdnNode*> node_list;
  bdnmgr.sort(node_list);
  for (vector<const BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* node0 = node->fanin(0);
    const BdnNode* node1 = node->fanin(1);
    const BdnNode* node00 = node0->fanin(0);
    const BdnNode* node01 = node0->fanin(1);
    const BdnNode* node10 = node1->fanin(0);
    const BdnNode* node11 = node1->fanin(1);
    if ( node00 == node10 && node01 == node11 ) {
      if ( (node0->fanin_inv(0) ^ node0->fanin_inv(1)) &&
	   (node1->fanin_inv(0) ^ node1->fanin_inv(1)) ) {
      }
    }
    else if ( node00 == node11 && node01 == node10 ) {
      ;
    }
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス LsimNaive2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimNaive2::LsimNaive2()
{
}

// @brief デストラクタ
LsimNaive2::~LsimNaive2()
{
}

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimNaive2::set_network(const BdnMgr& bdn,
			const unordered_map<string, ymuint>& order_map)
{
  // コピー
  mBdnMgr = bdn;

  vector<SimNode*> map(mBdnMgr.max_node_id());

  const BdnNodeList& input_list = mBdnMgr.input_list();
  mInputList.clear();
  mInputList.resize(input_list.size());
  ymuint input_id = 0;
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p, ++ input_id) {
    const BdnNode* node = *p;
    map[node->id()] = &mInputList[input_id];
    //cout << "map[" << node->id() << "]" << endl;
  }

  // XORize する．
  xoring(mBdnMgr);

  ymuint lnum = mBdnMgr.lnode_num();
  mNodeList.clear();
  mNodeList.resize(lnum);
  vector<const BdnNode*> node_list;
  mBdnMgr.sort(node_list);
  ymuint id = 0;
  for (vector<const BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p, ++ id) {
    const BdnNode* node = *p;
    SimNode& snode = mNodeList[id];
    snode.mFanins[0] = map[node->fanin(0)->id()];
    snode.mFanins[1] = map[node->fanin(1)->id()];
    snode.mType = 0U;
    if ( node->is_xor() ) {
      snode.mType = 4U;
    }
    if ( node->fanin_inv(0) ) {
      snode.mType |= 1U;
    }
    if ( node->fanin_inv(1) ) {
      snode.mType |= 2U;
    }
    map[node->id()] = &snode;
  }

  const BdnNodeList& output_list = mBdnMgr.output_list();
  mOutputList.clear();
  mOutputList.reserve(output_list.size());
  mOutputInv.clear();
  mOutputInv.reserve(output_list.size());
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* inode = node->output_fanin();
    if ( inode != NULL ) {
      mOutputList.push_back(map[inode->id()]);
    }
    else {
      mOutputList.push_back(NULL);
    }
    mOutputInv.push_back(node->output_fanin_inv());
  }

  cout << "Node num: " << id << endl;
}

// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimNaive2::eval(const vector<ymuint64>& iv,
		 vector<ymuint64>& ov)
{
  ymuint ni = mInputList.size();
  ASSERT_COND( ni == iv.size() );
  for (ymuint i = 0; i < ni; ++ i) {
    mInputList[i].mVal = iv[i];
  }

  ymuint nn = mNodeList.size();
  for (ymuint i = 0; i < nn; ++ i) {
    SimNode& snode = mNodeList[i];
    SimNode* snode0 = snode.mFanins[0];
    ymuint64 val0 = snode0->mVal;
    if ( snode.mType & 1U ) {
      val0 = ~val0;
    }
    SimNode* snode1 = snode.mFanins[1];
    ymuint64 val1 = snode1->mVal;
    if ( snode.mType & 2U ) {
      val1 = ~val1;
    }
    ymuint64 val;
    if ( snode.mType & 4U ) {
      val = val0 ^ val1;
    }
    else {
      val = val0 & val1;
    }
    snode.mVal = val;
  }

  ymuint no = mOutputList.size();
  for (ymuint i = 0; i < no; ++ i) {
    SimNode* snode = mOutputList[i];
    ymuint64 val = 0UL;
    if ( snode != NULL ) {
      val = snode->mVal;
    }
    if ( mOutputInv[i] ) {
      val = ~val;
    }
    ov[i] = val;
  }
}

END_NAMESPACE_YM
