
/// @file BlifBdnConv.cc
/// @brief BlifBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BlifBdnConv.h"

#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnNodeHandle.h"
#include "YmNetworks/BdnDff.h"
#include "YmNetworks/BlifNetwork.h"
#include "YmNetworks/BlifNode.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

// @brief コンストラクタ
BlifBdnConv::BlifBdnConv()
{
}

// @brief デストラクタ
BlifBdnConv::~BlifBdnConv()
{
}

// @brief 変換する
// @param[in] blif_network 変換元のネットワーク
// @param[in] network 変換先のネットワーク
// @param[in] clock_name クロック信号のポート名
// @param[in] clear_name クリア信号のポート名
bool
BlifBdnConv::operator()(const BlifNetwork& blif_network,
			BdnMgr& network,
			const string& clock_name,
			const string& clear_name)
{
  mNetwork = &network;
  ymuint n = blif_network.max_node_id();
  mNodeMap.clear();
  mNodeMap.resize(n);
  mNodeFlag.clear();
  mNodeFlag.resize(n, false);

  // モデル名の設定
  mNetwork->set_name(blif_network.name());

  // 外部入力ノードの生成
  ymuint npi = blif_network.pi_num();
  for (ymuint i = 0; i < npi; ++ i) {
    const BlifNode* blif_node = blif_network.pi(i);
    BdnPort* port = mNetwork->new_input_port(blif_node->name(), 1);
    BdnNode* node = port->_input(0);
    put_node(blif_node, BdnNodeHandle(node, false));
  }

  // D-FFの生成
  ymuint nff = blif_network.ff_num();
  vector<BdnDff*> dff_array(nff);
  BdnNodeHandle clock_h;
  BdnNodeHandle clear_h;
  if ( nff > 0 ) {
    // クロック用の外部入力の生成
    BdnPort* clock_port = mNetwork->new_input_port(clock_name, 1);
    BdnNode* clock = clock_port->_input(0);
    clock_h = BdnNodeHandle(clock, false);

    // クリア用の外部入力の生成
    bool need_clear = false;
    for (ymuint i = 0; i < nff; ++ i) {
      const BlifNode* blif_node = blif_network.ff(i);
      int opat = blif_node->opat();
      if ( opat == '0' || opat == '1' ) {
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
  for (ymuint i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    bool has_preset = false;
    bool has_clear = false;
    if ( blif_node->opat() == '0' ) {
      has_clear = true;
    }
    else if ( blif_node->opat() == '1' ) {
      has_preset = true;
    }

    // D-FF の生成
    BdnDff* dff = mNetwork->new_dff(blif_node->name());
    dff_array[i] = dff;

    // D-FF の出力の登録
    BdnNode* node = dff->_output();
    put_node(blif_node, BdnNodeHandle(node, false));

    // クロック信号の設定
    BdnNode* dff_clock = dff->_clock();
    mNetwork->change_output_fanin(dff_clock, clock_h);

    // クリア(もしくはプリセット)信号の設定
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
  ymuint npo = blif_network.po_num();
  for (ymuint i = 0; i < npo; ++ i) {
    const BlifNode* blif_node = blif_network.po(i);
    BdnPort* port = mNetwork->new_output_port(blif_node->name(), 1);
    BdnNode* node = port->_output(0);
    BdnNodeHandle inode_h = make_node(blif_network, blif_node->id());
    mNetwork->change_output_fanin(node, inode_h);
  }

  // D-FFに用いられているノードを再帰的に生成
  for (ymuint i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    BdnNodeHandle inode_h = make_node(blif_network, blif_node->inode_id());
    BdnDff* dff = dff_array[i];
    BdnNode* dff_input = dff->_input();
    mNetwork->change_output_fanin(dff_input, inode_h);
  }

  return true;
}

// blif_node に対応するノードを作る．
BdnNodeHandle
BlifBdnConv::make_node(const BlifNetwork& blif_network,
		       ymuint node_id)
{
  const BlifNode* blif_node = blif_network.node(node_id);
  BdnNodeHandle node_handle;
  if ( !get_node(blif_node, node_handle) ) {
    assert_cond( blif_node->type() == BlifNode::kLogic, __FILE__, __LINE__);
    ymuint ni = blif_node->fanin_num();
    vector<BdnNodeHandle> fanins(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = make_node(blif_network, blif_node->fanin_id(i));
    }

    ymuint nc = blif_node->cube_num();
    if ( blif_node->opat() == '1' ) {
      vector<BdnNodeHandle> or_leaves;
      or_leaves.reserve(nc);
      for (ymuint c = 0; c < nc; ++ c) {
	vector<BdnNodeHandle> and_leaves;
	and_leaves.reserve(ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  char v = blif_node->cube_pat(c, i);
	  if ( v == '0' ) {
	    and_leaves.push_back(~fanins[i]);
	  }
	  else if ( v == '1' ) {
	    and_leaves.push_back(fanins[i]);
	  }
	}
	ymuint n = and_leaves.size();
	assert_cond( n > 0, __FILE__, __LINE__);
	or_leaves.push_back(mNetwork->new_and(and_leaves));
      }
      ymuint n = or_leaves.size();
      if ( n == 0 ) {
	node_handle = BdnNodeHandle::make_one();
      }
      else {
	node_handle = mNetwork->new_or(or_leaves);
      }
    }
    else {
      vector<BdnNodeHandle> and_leaves;
      and_leaves.reserve(nc);
      for (ymuint c = 0; c < nc; ++ c) {
	vector<BdnNodeHandle> or_leaves;
	or_leaves.reserve(ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  char v = blif_node->cube_pat(c, i);
	  if ( v == '0' ) {
	    or_leaves.push_back(fanins[i]);
	  }
	  else if ( v == '1' ) {
	    or_leaves.push_back(~fanins[i]);
	  }
	}
	ymuint n = or_leaves.size();
	assert_cond( n > 0, __FILE__, __LINE__);
	and_leaves.push_back(mNetwork->new_or(or_leaves));
      }
      ymuint n = and_leaves.size();
      if ( n == 0 ) {
	node_handle = BdnNodeHandle::make_zero();
      }
      else {
	node_handle = mNetwork->new_and(and_leaves);
      }
    }
    put_node(blif_node, node_handle);
  }
  return node_handle;
}

// @brief blif_node に対応した BdnNode を取り出す．
bool
BlifBdnConv::get_node(const BlifNode* blif_node,
		      BdnNodeHandle& node_handle)
{
  ymuint id = blif_node->id();
  if ( mNodeFlag[id] ) {
    node_handle = mNodeMap[id];
    return true;
  }
  return false;
}

// @brief blif_node に対応した BdnNode を登録する．
void
BlifBdnConv::put_node(const BlifNode* blif_node,
		      BdnNodeHandle node_handle)
{
  ymuint id = blif_node->id();
  mNodeMap[id] = node_handle;
  mNodeFlag[id] = true;
}

END_NAMESPACE_YM_NETWORKS_BLIF
