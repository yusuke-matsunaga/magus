
/// @file BNetBdnConv.cc
/// @brief BNetBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BNetBdnConv.h"
#include "ym_networks/BNetwork.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnNodeHandle.h"
#include "ym_networks/BdnDff.h"


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
// @param[in] clear_name クリア信号のポート名
bool
BNetBdnConv::operator()(const BNetwork& bnetwork,
			BdnMgr& network,
			const string& clock_name,
			const string& clear_name)
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
    BdnPort* port = mNetwork->new_input_port(bnode->name(), 1);
    BdnNode* node = port->_input(0);
    put_node(bnode, BdnNodeHandle(node, false));
  }

  // D-FFの生成
  ymuint nff = bnetwork.latch_node_num();
  vector<BdnDff*> dff_array(nff);
  BdnNodeHandle clock_h;
  BdnNodeHandle clear_h;
  if ( nff > 0 ) {
    // クロック用の外部入力の生成
    BdnPort* clock_port = mNetwork->new_input_port(clock_name, 1);
    BdnNode* clock = clock_port->_input(0);
    clock_h = BdnNodeHandle(clock, false);

    // リセット用の外部入力の生成
    bool need_clear = false;
    for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
	 p != bnetwork.latch_nodes_end(); ++ p) {
      const BNode* bnode = *p;
      int rval = bnode->reset_value();
      if ( rval == 0 || rval == 1 ) {
	need_clear = true;
	break;
      }
    }
    if ( need_clear ) {
      BdnPort* clear_port = mNetwork->new_input_port(clear_name, 1);
      BdnNode* clear = clear_port->_input(0);
      clear_h = BdnNodeHandle(clear, false);
    }
  }
  ymuint i = 0;
  for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
       p != bnetwork.latch_nodes_end(); ++ p, ++ i) {
    const BNode* bnode = *p;
    bool has_clear = false;
    bool has_preset = false;
    int rval = bnode->reset_value();
    if ( rval == 0 ) {
      has_clear = true;
    }
    else if ( rval == 1 ) {
      has_preset = true;
    }

    // D-FF の生成
    BdnDff* dff = mNetwork->new_dff(bnode->name());
    dff_array[i] = dff;

    // D-FF の出力の登録
    BdnNode* node = dff->_output();
    put_node(bnode, BdnNodeHandle(node, false));

    // クロック信号の設定
    BdnNode* dff_clock = dff->_clock();
    mNetwork->change_output_fanin(dff_clock, clock_h);

    // リセット(もしくはセット)信号の設定
    if ( has_clear ) {
      BdnNode* dff_clear = dff->_clear();
      mNetwork->change_output_fanin(dff_clear, clear_h);
    }
    else if ( has_preset ) {
      BdnNode* dff_preset = dff->_preset();
      mNetwork->change_output_fanin(dff_preset, clear_h);
    }
  }

  // 外部出力に用いられているノードを再帰的に生成
  for (BNodeList::const_iterator p = bnetwork.outputs_begin();
       p != bnetwork.outputs_end(); ++ p) {
    const BNode* bnode = *p;
    BdnPort* port = mNetwork->new_output_port(bnode->name(), 1);
    BdnNode* node = port->_output(0);
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
    BdnNode* dff_input = dff->_input();
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
