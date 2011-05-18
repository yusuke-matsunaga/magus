
/// @file libym_networks/BlifBdnConv.cc
/// @brief BlifBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBdnConv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BlifBdnConv.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnNodeHandle.h"
#include "ym_networks/BdnDff.h"
#include "ym_blif/BlifNetwork.h"


BEGIN_NAMESPACE_YM_BDN

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
// @param[in] reset_name リセット信号のポート名
bool
BlifBdnConv::operator()(const BlifNetwork& blif_network,
			BdnMgr& network,
			const string& clock_name,
			const string& reset_name)
{
  mNetwork = &network;
  ymuint n = blif_network.max_node_id();
  mNodeMap.resize(n);
  mNodeFlag.resize(n, false);

  // モデル名の設定
  mNetwork->set_name(blif_network.name());

  // 外部入力ノードの生成
  ymuint npi = blif_network.npi();
  for (ymuint i = 0; i < npi; ++ i) {
    const BlifNode* blif_node = blif_network.pi(i);
    BdnNode* node = mNetwork->new_port_input(blif_node->name());
    put_node(blif_node, BdnNodeHandle(node, false));
  }

  // D-FFの生成
  ymuint nff = blif_network.nff();
  vector<BdnDff*> dff_array(nff);
  BdnNodeHandle clock_h;
  BdnNodeHandle reset_h;
  if ( nff > 0 ) {
    // クロック用の外部入力の生成
    BdnNode* clock = mNetwork->new_port_input(clock_name);
    clock_h = BdnNodeHandle(clock, false);

    // リセット用の外部入力の生成
    bool need_reset = false;
    for (ymuint i = 0; i < nff; ++ i) {
      const BlifNode* blif_node = blif_network.ff(i);
      int opat = blif_node->opat();
      if ( opat == '0' || opat == '1' ) {
	need_reset = true;
	break;
      }
    }
    if ( need_reset ) {
      BdnNode* reset = mNetwork->new_port_input(reset_name);
      reset_h = BdnNodeHandle(reset, false);
    }
  }
  for (ymuint i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    bool has_set = false;
    bool has_reset = false;
    if ( blif_node->opat() == '0' ) {
      has_reset = true;
    }
    else if ( blif_node->opat() == '1' ) {
      has_set = true;
    }

    // D-FF の生成
    BdnDff* dff = mNetwork->new_dff(blif_node->name());
    dff_array[i] = dff;

    // D-FF の出力の登録
    BdnNode* node = dff->output();
    put_node(blif_node, BdnNodeHandle(node, false));

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
  ymuint npo = blif_network.npo();
  for (ymuint i = 0; i < npo; ++ i) {
    const BlifNode* blif_node = blif_network.po(i);
    BdnNode* node = mNetwork->new_port_output(blif_node->name());
    BdnNodeHandle inode_h = make_node(blif_node);
    mNetwork->change_output_fanin(node, inode_h);
  }

  // D-FFに用いられているノードを再帰的に生成
  for (ymuint i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    BdnNodeHandle inode_h = make_node(blif_node->fanin(0));
    BdnDff* dff = dff_array[i];
    BdnNode* dff_input = dff->input();
    mNetwork->change_output_fanin(dff_input, inode_h);
  }

  return true;
}

// blif_node に対応するノードを作る．
BdnNodeHandle
BlifBdnConv::make_node(const BlifNode* blif_node)
{
  BdnNodeHandle node_handle;
  if ( !get_node(blif_node, node_handle) ) {
    assert_cond( blif_node->type() == BlifNode::kLogic, __FILE__, __LINE__);
    ymuint ni = blif_node->ni();
    vector<BdnNodeHandle> fanins(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = make_node(blif_node->fanin(i));
    }

    ymuint nc = blif_node->nc();
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

END_NAMESPACE_YM_BDN
