
/// @file LsimMpx2.cc
/// @brief LsimMpx2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimMpx2.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnPort.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimMpx2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimMpx2::LsimMpx2() :
  mBddMgr("bmc", "Bdd Manager")
{
}

// @brief デストラクタ
LsimMpx2::~LsimMpx2()
{
}

BEGIN_NONAMESPACE

inline
ympuint
encode(LsimMpx2::MpxNode* node,
       bool inv)
{
  return reinterpret_cast<ympuint>(node) | inv;
}

inline
LsimMpx2::MpxNode*
decode_node(ympuint val)
{
  return reinterpret_cast<LsimMpx2::MpxNode*>(val & ~1UL);
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
  LsimMpx2::MpxNode* node = decode_node(ptr);
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
LsimMpx2::set_network(const BdnMgr& bdn,
		      const hash_map<string, ymuint>& order_map)
{
  ymuint n = bdn.max_node_id();
  vector<Bdd> bddmap(n);

  hash_map<Bdd, ympuint> mpx_map;

  const BdnNodeList& input_list = bdn.input_list();
  ymuint ni = input_list.size();
  if ( order_map.empty() ) {
    ymuint id = 0;
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      Bdd bdd = mBddMgr.make_posiliteral(id);
      ++ id;
      bddmap[node->id()] = bdd;
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

  ymuint nbdd = size(output_bdd_list);

  cout << "BDD size: " << nbdd << endl;

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
LsimMpx2::make_mpx(Bdd bdd,
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
  ymuint varid1 = bdd.root_decomp(bdd0, bdd1);

  ymuint varid2 = 0;
  ympuint node00;
  ympuint node01;
  if ( bdd0.is_zero() ) {
    node00 = node01 = 0UL;
  }
  else if ( bdd0.is_one() ) {
    node00 = node01 = 1UL;
  }
  else {
    Bdd bdd00;
    Bdd bdd01;
    varid2 = bdd0.root_decomp(bdd00, bdd01);
    node00 = make_mpx(bdd00, mpx_map);
    node01 = make_mpx(bdd01, mpx_map);
  }

  ymuint varid3 = 0;
  ympuint node10;
  ympuint node11;
  if ( bdd1.is_zero() ) {
    node10 = node11 = 0UL;
  }
  else if ( bdd1.is_one() ) {
    node10 = node11 = 1UL;
  }
  else {
    Bdd bdd10;
    Bdd bdd11;
    varid3 = bdd1.root_decomp(bdd10, bdd11);
    node10 = make_mpx(bdd10, mpx_map);
    node11 = make_mpx(bdd11, mpx_map);
  }

  mNodeList.push_back(MpxNode(varid1, varid2, varid3,
			      node00, node01, node10, node11));
  ympuint ptr = encode(&mNodeList.back(), false);
  mpx_map.insert(make_pair(bdd, ptr));

  return ptr;
}

// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimMpx2::eval(const vector<ymuint64>& iv,
	       vector<ymuint64>& ov)
{
  ymuint nn = mNodeList.size();
  for (ymuint i = 0; i < nn; ++ i) {
    MpxNode& node = mNodeList[i];
    ympuint ptr00 = node.mFanins[0];
    ymuint64 val00 = ptr_eval(ptr00);
    ympuint ptr01 = node.mFanins[1];
    ymuint64 val01 = ptr_eval(ptr01);
    ympuint ptr10 = node.mFanins[2];
    ymuint64 val10 = ptr_eval(ptr10);
    ympuint ptr11 = node.mFanins[3];
    ymuint64 val11 = ptr_eval(ptr11);

    ymuint64 c_val1 = iv[node.mId[0]];
    ymuint64 c_val2 = iv[node.mId[1]];
    ymuint64 c_val3 = iv[node.mId[2]];

    ymuint64 val0 = (c_val2 & val01) | (~c_val2 & val00);
    ymuint64 val1 = (c_val3 & val11) | (~c_val3 & val10);
    node.mVal = (c_val1 & val1) | (~c_val1 & val0);
  }

  ymuint no = ov.size();
  for (ymuint i = 0; i < no; ++ i) {
    ympuint ptr = mOutputList[i];
    ov[i] = ptr_eval(ptr);
  }
}

END_NAMESPACE_YM
