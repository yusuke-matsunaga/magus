
/// @file LsimMpx.cc
/// @brief LsimMpx の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimMpx.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnPort.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimMpx
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimMpx::LsimMpx() :
  mBddMgr("bmc", "Bdd Manager")
{
}

// @brief デストラクタ
LsimMpx::~LsimMpx()
{
}

BEGIN_NONAMESPACE

inline
ympuint
encode(LsimMpx::MpxNode* node,
       bool inv)
{
  return reinterpret_cast<ympuint>(node) | inv;
}

inline
LsimMpx::MpxNode*
decode_node(ympuint val)
{
  return reinterpret_cast<LsimMpx::MpxNode*>(val & ~1UL);
}

inline
bool
decode_inv(ympuint val)
{
  return static_cast<bool>(val & 1UL);
}

inline
ymuint64
ptr_eval(ympuint ptr)
{
  LsimMpx::MpxNode* node = decode_node(ptr);
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

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimMpx::set_network(const BdnMgr& bdn,
		     const hash_map<string, ymuint>& order_map)
{
  ymuint n = bdn.max_node_id();
  vector<Bdd> bddmap(n);

  hash_map<Bdd, ympuint> mpx_map;

  const BdnNodeList& input_list = bdn.input_list();
  ymuint ni = input_list.size();
  mInputList.clear();
  mInputList.reserve(ni);
  if ( order_map.empty() ) {
    ymuint id = 0;
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      Bdd bdd = mBddMgr.make_posiliteral(id);
      ++ id;
      bddmap[node->id()] = bdd;
      mInputList.push_back(MpxNode(id, 0UL, 0UL));
      ympuint ptr = encode(&mInputList.back(), false);
      mpx_map.insert(make_pair(bdd, ptr));
    }
  }
  else {
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      string name = node->port()->name();
      hash_map<string, ymuint>::const_iterator q = order_map.find(name);
      if ( q == order_map.end() ) {
	cerr << "No map for " << name << endl;
	abort();
      }
      ymuint id = q->second;
      Bdd bdd = mBddMgr.make_posiliteral(id);
      bddmap[node->id()] = bdd;
      mInputList.push_back(MpxNode(id, 0UL, 0UL));
      ympuint ptr = encode(&mInputList.back(), false);
      mpx_map.insert(make_pair(bdd, ptr));
    }
  }

  vector<BdnNode*> node_list;
  bdn.sort(node_list);
  for (vector<BdnNode*>::const_iterator p = node_list.begin();
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
  vector<Bdd> output_bdd_list;
  output_bdd_list.reserve(output_list.size());
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
    output_bdd_list.push_back(bdd);
  }

  cout << "BDD size: " << size(output_bdd_list) << endl;

  ymuint nbdd = size(output_bdd_list);
  mNodeList.clear();
  mNodeList.reserve(nbdd);
  mOutputList.clear();
  mOutputList.reserve(output_list.size());
  for (vector<Bdd>::iterator p = output_bdd_list.begin();
       p != output_bdd_list.end(); ++ p) {
    ympuint node = make_mpx(*p, mpx_map);
    mOutputList.push_back(node);
  }

  cout << "MPX size: " << mNodeList.size() << endl;
}

ympuint
LsimMpx::make_mpx(Bdd bdd,
		  hash_map<Bdd, ympuint>& mpx_map)
{
  if ( bdd.is_zero() ) {
    return 0UL;
  }
  if ( bdd.is_one() ) {
    return 1UL;
  }
  hash_map<Bdd, ympuint>::iterator p = mpx_map.find(bdd);
  if ( p != mpx_map.end() ) {
    return p->second;
  }

  p = mpx_map.find(~bdd);
  if ( p != mpx_map.end() ) {
    return p->second ^ 1UL;
  }

  Bdd bdd0;
  Bdd bdd1;
  ymuint varid = bdd.root_decomp(bdd0, bdd1);
  ympuint node0 = make_mpx(bdd0, mpx_map);
  ympuint node1 = make_mpx(bdd1, mpx_map);

  mNodeList.push_back(MpxNode(varid, node0, node1));
  ympuint ptr = encode(&mNodeList.back(), false);
  mpx_map.insert(make_pair(bdd, ptr));

  return ptr;
}

// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimMpx::eval(const vector<ymuint64>& iv,
	      vector<ymuint64>& ov)
{
  ymuint ni = mInputList.size();
  assert_cond( ni == iv.size(), __FILE__, __LINE__);

  ymuint nn = mNodeList.size();
  for (ymuint i = 0; i < nn; ++ i) {
    MpxNode& node = mNodeList[i];
    ympuint ptr0 = node.mFanins[0];
    ymuint64 val0 = ptr_eval(ptr0);
    ympuint ptr1 = node.mFanins[1];
    ymuint64 val1 = ptr_eval(ptr1);
    ymuint64 c_val = iv[node.mId];

    node.mVal = (c_val & val1) | (~c_val & val0);
  }

  ymuint no = ov.size();
  for (ymuint i = 0; i < no; ++ i) {
    ympuint ptr = mOutputList[i];
    ov[i] = ptr_eval(ptr);
  }
}

END_NAMESPACE_YM
