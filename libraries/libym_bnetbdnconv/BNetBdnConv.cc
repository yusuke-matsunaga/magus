
/// @file libym_bdn/BNetBdnConv.cc
/// @brief BNetBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetBdnConv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnetbdnconv/BNetBdnConv.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bdn/BdnMgr.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"
#include "ym_bdn/BdnDff.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
BNetBdnConv::BNetBdnConv()
{
}

// @brief デストラクタ
BNetBdnConv::~BNetBdnConv()
{
}

// @brief 変換する
// @param[in] bnetwork 変換元のネットワーク
// @param[in] network 変換先のネットワーク
// @param[in] clock_name クロック信号のポート名
// @param[in] reset_name リセット信号のポート名
bool
BNetBdnConv::operator()(const BNetwork& bnetwork,
			BdnMgr& network,
			const string& clock_name,
			const string& reset_name)
{
  mNetwork = &network;
  ymuint n = bnetwork.max_node_id();
  mNodeMap.resize(n);
  mNodeFlag.resize(n, false);

  // モデル名の設定
  mNetwork->set_name(bnetwork.model_name());

  // 外部入力ノードの生成
  for (BNodeList::const_iterator p = bnetwork.inputs_begin();
       p != bnetwork.inputs_end(); ++ p) {
    const BNode* bnode = *p;
    BdnNode* node = mNetwork->new_port_input(bnode->name());
    put_node(bnode, BdnNodeHandle(node, false));
  }

  // D-FFの生成
  ymuint nff = bnetwork.latch_node_num();
  vector<BdnDff*> dff_array(nff);
  BdnNodeHandle clock_h;
  BdnNodeHandle reset_h;
  if ( nff > 0 ) {
    // クロック用の外部入力の生成
    BdnNode* clock = mNetwork->new_port_input(clock_name);
    clock_h = BdnNodeHandle(clock, false);

    // リセット用の外部入力の生成
    bool need_reset = false;
    for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
	 p != bnetwork.latch_nodes_end(); ++ p) {
      const BNode* bnode = *p;
      int rval = bnode->reset_value();
      if ( rval == 0 || rval == 1 ) {
	need_reset = true;
	break;
      }
    }
    if ( need_reset ) {
      BdnNode* reset = mNetwork->new_port_input(reset_name);
      reset_h = BdnNodeHandle(reset, false);
    }
  }
  ymuint i = 0;
  for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
       p != bnetwork.latch_nodes_end(); ++ p, ++ i) {
    const BNode* bnode = *p;
    bool has_set = false;
    bool has_reset = false;
    int rval = bnode->reset_value();
    if ( rval == 0 ) {
      has_reset = true;
    }
    else if ( rval == 1 ) {
      has_set = true;
    }

    // D-FF の生成
    BdnDff* dff = mNetwork->new_dff(bnode->name());
    dff_array[i] = dff;

    // D-FF の出力の登録
    BdnNode* node = dff->output();
    put_node(bnode, BdnNodeHandle(node, false));

    // クロック信号の設定
    BdnNode* dff_clock = dff->clock();
    mNetwork->change_output_fanin(dff_clock, clock_h);

    // リセット(もしくはセット)信号の設定
    if ( has_reset ) {
      BdnNode* dff_reset = dff->reset();
      mNetwork->change_output_fanin(dff_reset, reset_h);
    }
    else if ( has_set ) {
      BdnNode* dff_set = dff->set();
      mNetwork->change_output_fanin(dff_set, reset_h);
    }
  }

  // 外部出力に用いられているノードを再帰的に生成
  for (BNodeList::const_iterator p = bnetwork.outputs_begin();
       p != bnetwork.outputs_end(); ++ p) {
    const BNode* bnode = *p;
    BdnNode* node = mNetwork->new_port_output(bnode->name());
    BdnNodeHandle inode_h = make_node(bnode->fanin(0));
    mNetwork->change_output_fanin(node, inode_h);
  }

  // D-FFに用いられているノードを再帰的に生成
  i = 0;
  for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
       p != bnetwork.latch_nodes_end(); ++ p, ++ i) {
    const BNode* bnode = *p;
    BdnNodeHandle inode_h = make_node(bnode->fanin(0));
    BdnDff* dff = dff_array[i];
    BdnNode* dff_input = dff->input();
    mNetwork->change_output_fanin(dff_input, inode_h);
  }

  return true;
}


BEGIN_NONAMESPACE

BdnNodeHandle
make_node_sub(BdnMgr& bdn,
	      LogExpr func,
	      const vector<BdnNodeHandle>& fanins)
{
  if ( func.is_zero() ) {
    return BdnNodeHandle::make_zero();
  }
  if ( func.is_one() ) {
    return BdnNodeHandle::make_one();
  }
  if ( func.is_posiliteral() ) {
    return fanins[func.varid()];
  }
  if ( func.is_negaliteral() ) {
    return ~fanins[func.varid()];
  }
  // 以降は AND/OR/XOR
  ymuint n = func.child_num();
  vector<BdnNodeHandle> child_nodes(n);
  for (ymuint i = 0; i < n; ++ i) {
    child_nodes[i] = make_node_sub(bdn, func.child(i), fanins);
  }
  if ( func.is_and() ) {
    return bdn.new_and(child_nodes);
  }
  if ( func.is_or() ) {
    return bdn.new_or(child_nodes);
  }
  if ( func.is_xor() ) {
    return bdn.new_xor(child_nodes);
  }
  assert_not_reached(__FILE__, __LINE__);
  return BdnNodeHandle(); // ダミー
}

END_NONAMESPACE

// bnode に対応するノードを作る．
BdnNodeHandle
BNetBdnConv::make_node(const BNode* bnode)
{
  BdnNodeHandle node_handle;
  if ( !get_node(bnode, node_handle) ) {
    assert_cond( bnode->is_logic(), __FILE__, __LINE__);
    ymuint ni = bnode->ni();
    vector<BdnNodeHandle> fanins(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = make_node(bnode->fanin(i));
    }

    LogExpr func = bnode->func();
    node_handle = make_node_sub(*mNetwork, func, fanins);
    put_node(bnode, node_handle);
  }
  return node_handle;
}

// @brief bnode に対応した BdnNode を取り出す．
bool
BNetBdnConv::get_node(const BNode* bnode,
		      BdnNodeHandle& node_handle)
{
  ymuint id = bnode->id();
  if ( mNodeFlag[id] ) {
    node_handle = mNodeMap[id];
    return true;
  }
  return false;
}

// @brief bnode に対応した BdnNode を登録する．
void
BNetBdnConv::put_node(const BNode* bnode,
		      BdnNodeHandle node_handle)
{
  ymuint id = bnode->id();
  mNodeMap[id] = node_handle;
  mNodeFlag[id] = true;
}

END_NAMESPACE_YM
