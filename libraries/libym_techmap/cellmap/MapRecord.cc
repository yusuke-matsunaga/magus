
/// @file libym_techmap/cellmap/MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnDff.h"
#include "ym_networks/BdnLatch.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/CmnMgr.h"
#include "ym_networks/CmnPort.h"
#include "ym_networks/CmnDff.h"
#include "ym_networks/CmnLatch.h"

#include "Match.h"


BEGIN_NAMESPACE_YM_CELLMAP

// コンストラクタ
MapRecord::MapRecord()
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
// @param[in] sbjgraph サブジェクトグラフ
void
MapRecord::init(const BdnMgr& sbjgraph)
{
  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id() * 2);
}

// @brief マッチの情報だけコピーする．
void
MapRecord::copy(const MapRecord& src)
{
  ymuint n = src.mNodeInfo.size();
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mMatch = src.mNodeInfo[i].mMatch;
  }
}

// @brief D-FF のマッチを記録する．
// @param[in] dff D-FF
// @param[in] cell セル
void
MapRecord::set_dff_match(const BdnDff* dff,
			 const CmnDffCell* cell)
{
  mDffInfo[dff->id()] = cell;
}

// @brief ラッチのマッチを記録する．
// @param[in] latch ラッチ
// @param[in] cell セル
void
MapRecord::set_latch_match(const BdnLatch* latch,
			   const CmnLatchCell* cell)
{
  mLatchInfo[latch->id()] = cell;
}

// @brief マッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] match 対応するマッチ
// @param[in] cell セル
void
MapRecord::set_match(const BdnNode* node,
		     bool inv,
		     const Match& match,
		     const Cell* cell)
{
  NodeInfo& ni = node_info(node, inv);
  ni.mMatch = match;
  ni.mCell = cell;
}

// @brief インバータのマッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] cell セル
void
MapRecord::set_inv_match(const BdnNode* node,
			 bool inv,
			 const Cell* cell)
{
  NodeInfo& ni = node_info(node, inv);
  ni.mMatch.resize(1);
  ni.mMatch.set_leaf(0, node, !inv);
  ni.mCell = cell;
}

// @brief マッチを取り出す．
// @param[in] node 該当のノード
// @param[in] inv 極性
const Match&
MapRecord::get_match(const BdnNode* node,
		     bool inv)
{
  return node_info(node, inv).mMatch;
}

// @brief マッピング結果を CmnMgr にセットする．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] const0_cell 定数0のセル
// @param[in] const1_cell 定数1のセル
// @param[out] mapgraph マッピング結果を格納するネットワーク
void
MapRecord::gen_mapgraph(const BdnMgr& sbjgraph,
			const Cell* const0_cell,
			const Cell* const1_cell,
			CmnMgr& mapgraph)
{
  mapgraph.clear();

  mapgraph.set_name(sbjgraph.name());

  // ポートの生成
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BdnPort* sbjport = sbjgraph.port(i);
    ymuint nb = sbjport->bit_width();
    vector<ymuint> iovect(nb);
    sbjport->get_iovect(iovect);
    CmnPort* dst_port = mapgraph.new_port(sbjport->name(), iovect);
    for (ymuint j = 0; j < nb; ++ j) {
      const BdnNode* sbj_input = sbjport->input(j);
      if ( sbj_input ) {
	CmnNode* input = dst_port->_input(j);
	assert_cond( input != NULL, __FILE__, __LINE__);
	node_info(sbj_input, false).mMapNode = input;
      }
      const BdnNode* sbj_output = sbjport->output(j);
      if ( sbj_output ) {
	CmnNode* output = dst_port->_output(j);
	assert_cond( output != NULL, __FILE__, __LINE__);
	node_info(sbj_output, false).mMapNode = output;
      }
    }
  }

  // D-FF の生成
  const BdnDffList& dff_list = sbjgraph.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* sbj_dff = *p;
    const CmnDffCell* dff_cell = dff_info(sbj_dff);
    CmnDff* dff = mapgraph.new_dff(dff_cell, sbj_dff->name());

    const BdnNode* sbj_output = sbj_dff->output();
    CmnNode* output1 = dff->_output1();
    node_info(sbj_output, false).mMapNode = output1;
    CmnNode* output2 = dff->_output2();
    node_info(sbj_output, true).mMapNode = output2;

    const BdnNode* sbj_input = sbj_dff->input();
    CmnNode* input = dff->_input();
    node_info(sbj_input, false).mMapNode = input;

    const BdnNode* sbj_clock = sbj_dff->clock();
    CmnNode* clock = dff->_clock();
    node_info(sbj_clock, false).mMapNode = clock;

    const BdnNode* sbj_clear = sbj_dff->clear();
    if ( sbj_clear ) {
      CmnNode* clear = dff->_clear();
      assert_cond( clear != NULL, __FILE__, __LINE__);
      node_info(sbj_clear, false).mMapNode = clear;
    }

    const BdnNode* sbj_preset = sbj_dff->preset();
    if ( sbj_preset ) {
      CmnNode* preset = dff->_preset();
      assert_cond( preset != NULL, __FILE__, __LINE__);
      node_info(sbj_preset, false).mMapNode = preset;
    }
  }

  // ラッチの生成
  const BdnLatchList& latch_list = sbjgraph.latch_list();
  for (BdnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const BdnLatch* latch = *p;
  }

#if 0
  // 外部入力の生成
  const BdnNodeList& input_list = sbjgraph.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    CmnNode* mapnode = mapgraph.new_input();
    NodeInfo& node_info = this->node_info(node, false);
    node_info.mMapNode = mapnode;
  }
#endif

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  const BdnNodeList& output_list = sbjgraph.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* onode = *p;
    const BdnNode* node = onode->fanin(0);
    bool inv = onode->output_fanin_inv();
    CmnNode* mapnode = NULL;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph);
    }
    else {
      if ( inv ) {
	// 定数1ノードを作る．
	mapnode = mapgraph.new_logic(vector<CmnNode*>(0), const1_cell);
      }
      else {
	// 定数0ノードを作る．
	mapnode = mapgraph.new_logic(vector<CmnNode*>(0), const0_cell);
      }
    }
    CmnNode* omapnode = node_info(onode, false).mMapNode;
    mapgraph.set_output_fanin(omapnode, mapnode);
  }

#if 0
  // DFFからバックトレースを行い全ノードの生成を行う．
  for (BdnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnNode* onode = *p;
    const BdnNode* node = onode->fanin_data();
    bool inv = onode->fanin_data_inv();
    CmnNode* mapnode = NULL;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph);
    }
    else {
      if ( inv ) {
	// 定数1ノードを作る．
	mapnode = mapgraph.new_logic(vector<CmnNode*>(0), const1_cell);
      }
      else {
	// 定数0ノードを作る．
	mapnode = mapgraph.new_logic(vector<CmnNode*>(0), const0_cell);
      }
    }
    CmnNode* omapnode = node_info(onode, false).mMapNode;
    mapgraph.set_dff_input(omapnode, mapnode);
    {
      const BdnNode* node = onode->fanin_clock();
      bool inv = onode->fanin_clock_inv();
      CmnNode* mapnode = NULL;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	CmnNode* omapnode = node_info(onode, false).mMapNode;
	mapgraph.set_dff_clock(omapnode, mapnode);
      }
    }
    {
      const BdnNode* node = onode->fanin_set();
      bool inv = onode->fanin_set_inv();
      CmnNode* mapnode = NULL;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	CmnNode* omapnode = node_info(onode, false).mMapNode;
	mapgraph.set_dff_set(omapnode, mapnode);
      }
    }
    {
      const BdnNode* node = onode->fanin_rst();
      bool inv = onode->fanin_rst_inv();
      CmnNode* mapnode = NULL;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	CmnNode* omapnode = node_info(onode, false).mMapNode;
	mapgraph.set_dff_rst(omapnode, mapnode);
      }
    }
  }
#endif
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
CmnNode*
MapRecord::back_trace(const BdnNode* node,
		      bool inv,
		      CmnMgr& mapnetwork)
{
  NodeInfo& node_info = this->node_info(node, inv);
  CmnNode* mapnode = node_info.mMapNode;
  if ( mapnode ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  // node を根とするマッチを取り出す．
  const Match& match = node_info.mMatch;

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = match.leaf_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    back_trace(inode, iinv, mapnetwork);
  }

  mTmpFanins.clear();
  mTmpFanins.resize(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    NodeInfo& inode_info = this->node_info(inode, iinv);
    CmnNode* imapnode = inode_info.mMapNode;
    mTmpFanins[i] = imapnode;
  }

  // 新しいノードを作り mNodeMap に登録する．
  assert_cond( node_info.mCell != NULL, __FILE__, __LINE__);
  mapnode = mapnetwork.new_logic(mTmpFanins, node_info.mCell);
  node_info.mMapNode = mapnode;

  return mapnode;
}

// @brief D-FF の割り当て情報を取り出す．
const CmnDffCell*
MapRecord::dff_info(const BdnDff* dff)
{
  return mDffInfo[dff->id()];
}

// @brief ラッチの割り当て情報を取り出す．
const CmnLatchCell*
MapRecord::latch_info(const BdnLatch* latch)
{
  return mLatchInfo[latch->id()];
}

// @brief NodeInfo を取り出す．
MapRecord::NodeInfo&
MapRecord::node_info(const BdnNode* node,
		     bool inv)
{
  return mNodeInfo[node->id() * 2 + static_cast<ymuint>(inv)];
}

END_NAMESPACE_YM_CELLMAP
