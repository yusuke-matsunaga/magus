
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
#include "ym_networks/CmnDffCell.h"


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
  mDffInfo.clear();
  mDffInfo.resize(sbjgraph.max_dff_id());
  mLatchInfo.clear();
  mLatchInfo.resize(sbjgraph.max_latch_id());
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
// @param[in] pos_array ピン情報の配列
// @param[in] phase 入力ピンの極性情報
void
MapRecord::set_dff_match(const BdnDff* dff,
			 const Cell* cell,
			 FFPosArray pos_array,
			 ymuint phase)
{
  DffInfo& dffinfo = mDffInfo[dff->id()];
  dffinfo.mCell = cell;
  dffinfo.mPosArray = pos_array;
  dffinfo.mPhase = phase;
}

// @brief ラッチのマッチを記録する．
// @param[in] latch ラッチ
// @param[in] cell セル
// @param[in] pos_array ピン情報の配列
void
MapRecord::set_latch_match(const BdnLatch* latch,
			   const Cell* cell,
			   LatchPosArray pos_array)
{
  LatchInfo& latchinfo = mLatchInfo[latch->id()];
  latchinfo.mCell = cell;
  latchinfo.mPosArray = pos_array;
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
  NodeInfo& node_info = get_node_info(node, inv);
  node_info.mMatch = match;
  node_info.mCell = cell;
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
  NodeInfo& node_info = get_node_info(node, inv);
  node_info.mMatch.resize(1);
  node_info.mMatch.set_leaf(0, node, !inv);
  node_info.mCell = cell;
}

// @brief マッチを取り出す．
// @param[in] node 該当のノード
// @param[in] inv 極性
const Match&
MapRecord::get_match(const BdnNode* node,
		     bool inv)
{
  return get_node_info(node, inv).mMatch;
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
	get_node_info(sbj_input, false).mMapNode = input;
      }
      const BdnNode* sbj_output = sbjport->output(j);
      if ( sbj_output ) {
	CmnNode* output = dst_port->_output(j);
	assert_cond( output != NULL, __FILE__, __LINE__);
	get_node_info(sbj_output, false).mMapNode = output;
      }
    }
  }

  // D-FF の生成
  const BdnDffList& dff_list = sbjgraph.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* sbj_dff = *p;
    DffInfo& dff_info = get_dff_info(sbj_dff);
    const Cell* cell = dff_info.mCell;
    const CmnDffCell* dff_cell = mapgraph.dff_cell(cell);
    if ( dff_cell == NULL ) {
      ymuint pos_array[6];
      const FFPosArray& pos_array_src = dff_info.mPosArray;
      pos_array[0] = pos_array_src.data_pos();
      pos_array[1] = pos_array_src.clock_pos() | (pos_array_src.clock_sense() << 3);
      pos_array[2] = pos_array_src.clear_pos() | (pos_array_src.clear_sense() << 3);
      pos_array[3] = pos_array_src.preset_pos() | (pos_array_src.preset_sense() << 3);
      pos_array[4] = pos_array_src.q_pos();
      pos_array[5] = pos_array_src.iq_pos();
      dff_cell = mapgraph.reg_dff_cell(cell, pos_array);

    }
    CmnDff* dff = mapgraph.new_dff(dff_cell, sbj_dff->name());

    const BdnNode* sbj_output = sbj_dff->output();
    CmnNode* output1 = dff->_output1();
    get_node_info(sbj_output, false).mMapNode = output1;
    CmnNode* output2 = dff->_output2();
    get_node_info(sbj_output, true).mMapNode = output2;

    const BdnNode* sbj_input = sbj_dff->input();
    CmnNode* input = dff->_input();
    get_node_info(sbj_input, false).mMapNode = input;

    const BdnNode* sbj_clock = sbj_dff->clock();
    CmnNode* clock = dff->_clock();
    get_node_info(sbj_clock, false).mMapNode = clock;

    const BdnNode* sbj_clear = sbj_dff->clear();
    if ( sbj_clear->output_fanin() ) {
      CmnNode* clear = dff->_clear();
      assert_cond( clear != NULL, __FILE__, __LINE__);
      get_node_info(sbj_clear, false).mMapNode = clear;
    }

    const BdnNode* sbj_preset = sbj_dff->preset();
    if ( sbj_preset->output_fanin() ) {
      CmnNode* preset = dff->_preset();
      assert_cond( preset != NULL, __FILE__, __LINE__);
      get_node_info(sbj_preset, false).mMapNode = preset;
    }
  }

  // ラッチの生成
  const BdnLatchList& latch_list = sbjgraph.latch_list();
  for (BdnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const BdnLatch* latch = *p;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  const BdnNodeList& output_list = sbjgraph.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* onode = *p;
    const BdnNode* node = onode->fanin(0);
    if ( onode->output_type() == BdnNode::kDFF_CLEAR ||
	 onode->output_type() == BdnNode::kDFF_PRESET ) {
      if ( node == NULL ) {
	continue;
      }
    }
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
    CmnNode* omapnode = get_node_info(onode, false).mMapNode;
    mapgraph.set_output_fanin(omapnode, mapnode);
  }
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
CmnNode*
MapRecord::back_trace(const BdnNode* node,
		      bool inv,
		      CmnMgr& mapnetwork)
{
  NodeInfo& node_info = get_node_info(node, inv);
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
    NodeInfo& inode_info = get_node_info(inode, iinv);
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
MapRecord::DffInfo&
MapRecord::get_dff_info(const BdnDff* dff)
{
  return mDffInfo[dff->id()];
}

// @brief ラッチの割り当て情報を取り出す．
MapRecord::LatchInfo&
MapRecord::get_latch_info(const BdnLatch* latch)
{
  return mLatchInfo[latch->id()];
}

// @brief NodeInfo を取り出す．
MapRecord::NodeInfo&
MapRecord::get_node_info(const BdnNode* node,
			 bool inv)
{
  return mNodeInfo[node->id() * 2 + static_cast<ymuint>(inv)];
}

END_NAMESPACE_YM_CELLMAP
