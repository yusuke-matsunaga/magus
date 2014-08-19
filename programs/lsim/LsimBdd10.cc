﻿
/// @file LsimBdd10.cc
/// @brief LsimBdd10 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimBdd10.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnPort.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimBdd10
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimBdd10::LsimBdd10() :
  mBddMgr("bmc", "Bdd Manager")
{
}

// @brief デストラクタ
LsimBdd10::~LsimBdd10()
{
}


BEGIN_NONAMESPACE

inline
ympuint
encode(LsimBdd10::Bdd10Node* node,
       bool inv)
{
  return reinterpret_cast<ympuint>(node) | inv;
}

inline
LsimBdd10::Bdd10Node*
decode_node(ympuint val)
{
  return reinterpret_cast<LsimBdd10::Bdd10Node*>(val & ~1UL);
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
LsimBdd10::set_network(const BdnMgr& bdn,
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

  unordered_map<Bdd, ympuint> node_map;

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
LsimBdd10::make_node(Bdd bdd,
		     unordered_map<Bdd, ympuint>& node_map)
{
  if ( bdd.is_zero() ) {
    return 0UL;
  }
  if ( bdd.is_one() ) {
    return 1UL;
  }

  unordered_map<Bdd, ympuint>::iterator p = node_map.find(bdd);
  if ( p != node_map.end() ) {
    return p->second;
  }

  p = node_map.find(~bdd);
  if ( p != node_map.end() ) {
    return p->second ^ 1UL;
  }

  VarVector sup_list;
  bdd.support(sup_list);

  Bdd10Node* node = new Bdd10Node;
  ymuint n = sup_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( i >= 10 ) {
      break;
    }
    node->mId[i] = sup_list[i].val();
  }
  for (ymuint i = n; i < 10; ++ i) {
    node->mId[i] = 0;
  }
  for (ymuint addr = 0U; addr < 1024; ++ addr) {
    unordered_map<VarId, ymuint> var_map;
    for (ymuint i = 0; i < 10; ++ i) {
      if ( addr & (1U << i) ) {
	var_map.insert(make_pair(sup_list[i], 1));
      }
      else {
	var_map.insert(make_pair(sup_list[i], 0));
      }
    }
    Bdd tmp_bdd = bdd;
    for ( ; ; ) {
      Bdd bdd0;
      Bdd bdd1;
      VarId id = tmp_bdd.root_decomp(bdd0, bdd1);
      unordered_map<VarId, ymuint>::iterator p = var_map.find(id);
      if ( p == var_map.end() ) {
	break;
      }
      ymuint val = p->second;
      if ( val ) {
	tmp_bdd = bdd1;
      }
      else {
	tmp_bdd = bdd0;
      }
    }
    ympuint ptr = make_node(tmp_bdd, node_map);
    node->mFanins[addr] = ptr;
  }
  mNodeList.push_back(node);
  ympuint ptr = encode(node, false);

  node_map.insert(make_pair(bdd, ptr));

  return ptr;
}

BEGIN_NONAMESPACE

ymuint total_step;
ymuint total_num;

ymuint64
eval_bdd(ympuint ptr0,
	 const vector<ymuint64>& iv)
{
  ymuint val = 0U;
  for (ymuint b = 0; b < 64; ++ b) {
    ympuint ptr = ptr0;
    ymuint64 bit = 1UL << b;
    ++ total_num;
    for ( ; ; ) {
      ++ total_step;
      if ( ptr == 0UL ) {
	break;
      }
      else if ( ptr == 1UL ) {
	val |= bit;
	break;
      }
      LsimBdd10::Bdd10Node* node = decode_node(ptr);
      bool inv = decode_inv(ptr);
      ymuint addr = 0U;
      for (ymuint i = 0; i < 10; ++ i) {
	ymuint id = node->mId[i];
	ymuint64 ival = iv[id];
	if ( ival & bit ) {
	  addr |= (1U << i);
	}
      }
      ptr = node->mFanins[addr];
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
LsimBdd10::eval(const vector<ymuint64>& iv,
	       vector<ymuint64>& ov)
{
  total_step = 0;
  total_num = 0;
  ymuint no = ov.size();
  for (ymuint i = 0; i < no; ++ i) {
    ympuint ptr = mOutputList[i];
    ov[i] = eval_bdd(ptr, iv);
  }
#if 0
  cout << "total_step = " << total_step << endl
       << "total_num  = " << total_num << endl
       << "ave. step  = " << static_cast<double>(total_step) / total_num << endl;
#endif
}

END_NAMESPACE_YM
