
/// @file LsimNaive3.cc
/// @brief LsimNaive3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimNaive3.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

inline
ympuint
encode(LsimNaive3::SimNode* node,
       bool inv)
{
  return reinterpret_cast<ympuint>(node) | inv;
}

inline
LsimNaive3::SimNode*
decode_node(ympuint val)
{
  return reinterpret_cast<LsimNaive3::SimNode*>(val & ~1UL);
}

inline
bool
decode_inv(ympuint val)
{
  return static_cast<bool>(val & 1UL);
}

inline
ymuint64
eval_ptr(ympuint ptr)
{
  LsimNaive3::SimNode* node = decode_node(ptr);
  ymuint64 val = 0UL;
  if ( node ) {
    val = node->mVal;
  }
  bool inv = decode_inv(ptr);
  if ( inv ) {
    val = ~val;
  }
  return val;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LsimNaive3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimNaive3::LsimNaive3() :
  mBdnMgr(NULL)
{
}

// @brief デストラクタ
LsimNaive3::~LsimNaive3()
{
}

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimNaive3::set_network(const BdnMgr& bdn,
			const unordered_map<string, ymuint>& order_map)
{
  mBdnMgr = &bdn;

  vector<SimNode*> map(mBdnMgr->max_node_id());

  const BdnNodeList& input_list = mBdnMgr->input_list();
  mInputList.clear();
  mInputList.resize(input_list.size());
  ymuint input_id = 0;
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p, ++ input_id) {
    const BdnNode* node = *p;
    map[node->id()] = &mInputList[input_id];
    //cout << "map[" << node->id() << "]" << endl;
  }

  ymuint lnum = mBdnMgr->lnode_num();
  vector<const BdnNode*> node_list;
  mBdnMgr->sort(node_list);

  ymuint n = 0;
  for (vector<const BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->is_xor() ) {
      n += 2;
    }
    ++ n;
  }
  mNodeList.clear();
  mNodeList.resize(n);

  ymuint id = 0;
  for (vector<const BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    SimNode* snode = &mNodeList[id];
    ++ id;
    SimNode* snode0 = map[node->fanin(0)->id()];
    bool inv0 = node->fanin_inv(0);
    SimNode* snode1 = map[node->fanin(1)->id()];
    bool inv1 = node->fanin_inv(1);

    if ( node->is_xor() ) {
      SimNode* snode01 = &mNodeList[id];
      ++ id;
      snode01->mFanins[0] = encode(snode0, !inv0);
      snode01->mFanins[1] = encode(snode1, inv1);
      SimNode* snode10 = &mNodeList[id];
      ++ id;
      snode10->mFanins[0] = encode(snode0, inv0);
      snode10->mFanins[1] = encode(snode1, !inv1);
      snode->mFanins[0] = encode(snode01, true);
      snode->mFanins[1] = encode(snode10, false);
    }
    else {
      snode->mFanins[0] = encode(snode0, inv0);
      snode->mFanins[1] = encode(snode1, inv1);
    }
    map[node->id()] = snode;
  }

  const BdnNodeList& output_list = mBdnMgr->output_list();
  mOutputList.clear();
  mOutputList.reserve(output_list.size());
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* inode = node->output_fanin();
    SimNode* snode = NULL;
    if ( inode != NULL ) {
      snode = map[inode->id()];
    }
    mOutputList.push_back(encode(snode, node->output_fanin_inv()));
  }

  cout << "Node num: " << id << endl;
}

// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimNaive3::eval(const vector<ymuint64>& iv,
		 vector<ymuint64>& ov)
{
  ymuint ni = mInputList.size();
  assert_cond( ni == iv.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < ni; ++ i) {
    mInputList[i].mVal = iv[i];
  }

  ymuint nn = mNodeList.size();
  for (ymuint i = 0; i < nn; ++ i) {
    SimNode& snode = mNodeList[i];
    ympuint ptr0 = snode.mFanins[0];
    ymuint64 val0 = eval_ptr(ptr0);
    ympuint ptr1 = snode.mFanins[1];
    ymuint64 val1 = eval_ptr(ptr1);
    snode.mVal = val0 & val1;
  }

  ymuint no = mOutputList.size();
  for (ymuint i = 0; i < no; ++ i) {
    ympuint ptr = mOutputList[i];
    ymuint64 val = eval_ptr(ptr);
    ov[i] = val;
  }
}

END_NAMESPACE_YM
