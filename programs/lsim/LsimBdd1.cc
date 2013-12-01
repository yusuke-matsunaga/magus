
/// @file LsimBdd1.cc
/// @brief LsimBdd1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimBdd1.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnPort.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimBdd1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimBdd1::LsimBdd1() :
  mBddMgr("bmc", "Bdd Manager")
{
}

// @brief デストラクタ
LsimBdd1::~LsimBdd1()
{
}


BEGIN_NONAMESPACE

inline
ympuint
encode(LsimBdd1::Bdd1Node* node,
       bool inv)
{
  return reinterpret_cast<ympuint>(node) | inv;
}

inline
LsimBdd1::Bdd1Node*
decode_node(ympuint val)
{
  return reinterpret_cast<LsimBdd1::Bdd1Node*>(val & ~1UL);
}

inline
bool
decode_inv(ympuint val)
{
  return static_cast<bool>(val & 1UL);
}

END_NONAMESPACE

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimBdd1::set_network(const BdnMgr& bdn,
		      const hash_map<string, ymuint>& order_map)
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
      hash_map<string, ymuint>::const_iterator q = order_map.find(name);
      if ( q == order_map.end() ) {
	cerr << "No order for " << name << endl;
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

  hash_map<Bdd, ympuint> node_map;

  const BdnNodeList& output_list = bdn.output_list();
  ymuint no = output_list.size();
  mOutputList.clear();
  mOutputList.reserve(no);
  mNodeList.clear();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BdnNode* node = *p;
    BdnNode* node0 = node->output_fanin();
    Bdd bdd;
    if ( node0 != NULL ) {
      bdd = bddmap[node0->id()];
    }
    else {
      bdd = mBddMgr.make_zero();
    }
    if ( node->output_fanin_inv() ) {
      bdd = ~bdd;
    }
    ympuint ptr = make_node(bdd, node_map);
    mOutputList.push_back(ptr);
  }

  cout << "Node size: " << mNodeList.size() << endl;
}

ympuint
LsimBdd1::make_node(Bdd bdd,
		    hash_map<Bdd, ympuint>& node_map)
{
  if ( bdd.is_zero() ) {
    return 0UL;
  }
  if ( bdd.is_one() ) {
    return 1UL;
  }

  hash_map<Bdd, ympuint>::iterator p = node_map.find(bdd);
  if ( p != node_map.end() ) {
    return p->second;
  }

  p = node_map.find(~bdd);
  if ( p != node_map.end() ) {
    return p->second ^ 1UL;
  }

  Bdd bdd0;
  Bdd bdd1;
  VarId varid0 = bdd.root_decomp(bdd0, bdd1);

  ympuint node0 = make_node(bdd0, node_map);
  ympuint node1 = make_node(bdd1, node_map);

#if 0
  mNodeList.push_back(Bdd1Node(varid0, varid1, node00, node01, node10, node11));
  ympuint ptr = encode(&mNodeList.back(), false);
#else
  Bdd1Node* node = new Bdd1Node(varid0, node0, node1);
  mNodeList.push_back(node);
  ympuint ptr = encode(node, false);
#endif

  node_map.insert(make_pair(bdd, ptr));

  return ptr;
}

BEGIN_NONAMESPACE

ymuint64
eval_bdd(ympuint ptr0,
	 const vector<ymuint64>& iv)
{
  ymuint val = 0U;
  for (ymuint b = 0; b < 64; ++ b) {
    ympuint ptr = ptr0;
    ymuint64 bit = 1UL << b;
    for ( ; ; ) {
      if ( ptr == 0UL ) {
	break;
      }
      else if ( ptr == 1UL ) {
	val |= bit;
	break;
      }
      LsimBdd1::Bdd1Node* node = decode_node(ptr);
      bool inv = decode_inv(ptr);
      VarId id = node->mId;
      ymuint64 ival = iv[id.val()];
      if ( ival & bit ) {
	ptr = node->mFanins[1];
      }
      else {
	ptr = node->mFanins[0];
      }
      if ( inv ) {
	ptr = ptr ^ 1UL;
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
LsimBdd1::eval(const vector<ymuint64>& iv,
	       vector<ymuint64>& ov)
{
  ymuint no = ov.size();
  for (ymuint i = 0; i < no; ++ i) {
    ympuint ptr = mOutputList[i];
    ov[i] = eval_bdd(ptr, iv);
  }
}

END_NAMESPACE_YM
