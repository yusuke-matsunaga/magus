
/// @file LsimNaive.cc
/// @brief LsimNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimNaive.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimNaive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimNaive::LsimNaive() :
  mBdnMgr(NULL)
{
}

// @brief デストラクタ
LsimNaive::~LsimNaive()
{
}

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
void
LsimNaive::set_network(const BdnMgr& bdn)
{
  mBdnMgr = &bdn;

  const BdnNodeList& input_list = mBdnMgr->input_list();
  mInputList.clear();
  mInputList.reserve(input_list.size());
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    BdnNode* node = *p;
    mInputList.push_back(node);
  }

  const BdnNodeList& output_list = mBdnMgr->output_list();
  mOutputList.clear();
  mOutputList.reserve(output_list.size());
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BdnNode* node = *p;
    mOutputList.push_back(node);
  }

  mNodeList.clear();
  mBdnMgr->sort(mNodeList);

  ymuint n = mBdnMgr->max_node_id();
  mValArray.clear();
  mValArray.resize(n);
}

// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimNaive::eval(const vector<ymuint64>& iv,
		vector<ymuint64>& ov)
{
  if ( mBdnMgr == NULL ) {
    return;
  }

  ymuint ni = mInputList.size();
  assert_cond( ni == iv.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* node = mInputList[i];
    mValArray[node->id()] = iv[i];
  }

  ymuint nn = mNodeList.size();
  for (ymuint i = 0; i < nn; ++ i) {
    const BdnNode* node = mNodeList[i];
    const BdnNode* node0 = node->fanin0();
    ymuint64 val0 = mValArray[node0->id()];
    if ( node->fanin0_inv() ) {
      val0 = ~val0;
    }
    const BdnNode* node1 = node->fanin1();
    ymuint64 val1 = mValArray[node1->id()];
    if ( node->fanin1_inv() ) {
      val1 = ~val1;
    }
    ymuint64 val;
    if ( node->is_and() ) {
      val = val0 & val1;
    }
    else {
      val = val0 ^ val1;
    }
    mValArray[node->id()] = val;
  }

  ymuint no = mOutputList.size();
  for (ymuint i = 0; i < no; ++ i) {
    const BdnNode* node = mOutputList[i];
    const BdnNode* inode = node->output_fanin();
    ymuint64 val = 0U;
    if ( inode ) {
      val = mValArray[inode->id()];
    }
    if ( node->output_fanin_inv() ) {
      val = ~val;
    }
    ov[i] = val;
  }
}

END_NAMESPACE_YM
