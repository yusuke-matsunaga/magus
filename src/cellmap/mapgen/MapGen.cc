﻿
/// @file MapGen.cc
/// @brief MapGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MapGen.h"
#include "MapRecord.h"
#include "ym/BnNetwork.h"
#include "SbjPort.h"


BEGIN_NAMESPACE_YM_CELLMAP

// コンストラクタ
MapGen::MapGen()
{
}

// デストラクタ
MapGen::~MapGen()
{
}

BEGIN_NONAMESPACE

inline
ymuint
encode(ymuint pos,
       ymuint sense)
{
  return pos | (sense << 3);
}

END_NONAMESPACE

// @brief マッピング結果を BnNetwork にセットする．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] record マッピング結果
// @param[out] mapgraph マッピング結果を格納するネットワーク
void
MapGen::generate(const SbjGraph& sbjgraph,
		 const MapRecord& record,
		 BnNetwork& mapgraph)
{
  mapgraph.clear();

  mapgraph.set_name(sbjgraph.name());

  mMapGraph = &mapgraph;

  mMapReqList.clear();
  mMapReqList.reserve(sbjgraph.output_num());

  // ポートの生成
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* sbjport = sbjgraph.port(i);
    gen_port(sbjport);
  }

  // D-FF の生成
  ymuint n_dff = sbjgraph.dff_num();
  for (ymuint i = 0; i < n_dff; ++ i) {
    const SbjDff* sbj_dff = sbjgraph.dff(i);
    gen_dff(sbj_dff);
  }

  // ラッチの生成
  ymuint n_latch = sbjgraph.latch_num();
  for (ymuint i = 0; i < n_latch; ++ i) {
    const SbjLatch* sbj_latch = sbjgraph.latch(i);
    gen_latch(sbj_latch);
  }

  // 組み合わせ回路の出力からバックトレースを行い全ノードの生成を行う．
  for (vector<MapReq>::iterator p = mMapReqList.begin();
       p != mMapReqList.end(); ++ p) {
    const SbjNode* onode = p->mNode;
    bool ext_inv = p->mInv;
    const SbjNode* node = onode->fanin(0);
    bool inv = onode->output_fanin_inv() ^ ext_inv;
    ymuint mapnode = 0;
    if ( node ) {
      mapnode = back_trace(node, inv);
    }
    else {
      ymuint node_id = 0;
      if ( inv ) {
	// 定数1ノードを作る．
	const Cell* const1_cell = record.const1_cell();
	ASSERT_COND( const1_cell != nullptr );
	mapnode = mMapGraph->new_primitive(string(), 0, kBnLt_C1, const1_cell);
      }
      else {
	// 定数0ノードを作る．
	const Cell* const0_cell = record.const0_cell();
	ASSERT_COND( const0_cell != nullptr );
	mapnode = mMapGraph->new_primitive(string(), 0, kBnLt_C0, const0_cell);
      }
    }
    ymuint omapnode = mNodeInfo[onode->id()].mMapNode;
    mMapGraph->connect(mapnode, omapnode, 0);
  }
}

// @brief ポートの生成を行う．
void
MapGen::gen_port(const SbjPort* sbj_port)
{
  ymuint nb = sbj_port->bit_width();
  vector<ymuint> iovect(nb);
  sbj_port->get_iovect(iovect);
  BnPort* dst_port = mMapGraph->new_port(sbj_port->name(), iovect);
  for (ymuint j = 0; j < nb; ++ j) {
    const SbjNode* sbj_input = sbj_port->input(j);
    if ( sbj_input ) {
      BnNode* input = dst_port->_input(j);
      ASSERT_COND( input != nullptr );
      get_node_info(sbj_input, false).mMapNode = input;
    }
    const SbjNode* sbj_output = sbj_port->output(j);
    if ( sbj_output ) {
      BnNode* output = dst_port->_output(j);
      ASSERT_COND( output != nullptr );
      get_node_info(sbj_output, false).mMapNode = output;
      add_mapreq(sbj_output, false);
    }
  }
}

// @brief D-FF の生成を行う．
void
MapGen::gen_dff(const SbjDff* sbj_dff)
{
  const DffInfo& dff_info0 = get_dff_info(sbj_dff, false);
  const DffInfo& dff_info1 = get_dff_info(sbj_dff, true);
  const Cell* cell = nullptr;
  CellFFInfo pin_info;
  bool inv = false;
  if ( dff_info0.mCell == nullptr ) {
    if ( dff_info1.mCell == nullptr ) {
      ASSERT_NOT_REACHED;
    }
    cell = dff_info1.mCell;
    pin_info = dff_info1.mPinInfo;
    inv = true;
  }
  else {
    cell = dff_info0.mCell;
    pin_info = dff_info0.mPinInfo;
  }
  const CmnDffCell* dff_cell = mMapGraph->dff_cell(cell);
  if ( dff_cell == nullptr ) {
    dff_cell = mMapGraph->reg_dff_cell(cell, pin_info);

  }
  CmnDff* dff = mMapGraph->new_dff(dff_cell, sbj_dff->name());

  const SbjNode* sbj_output = sbj_dff->output();
  BnNode* output1 = dff->_output1();
  ASSERT_COND( output1 != nullptr );
  get_node_info(sbj_output, inv).mMapNode = output1;
  BnNode* output2 = dff->_output2();
  ASSERT_COND( output2 != nullptr );
  get_node_info(sbj_output, !inv).mMapNode = output2;

  const SbjNode* sbj_input = sbj_dff->input();
  BnNode* input = dff->_input();
  get_node_info(sbj_input, false).mMapNode = input;
  add_mapreq(sbj_input, inv);

  const SbjNode* sbj_clock = sbj_dff->clock();
  BnNode* clock = dff->_clock();
  get_node_info(sbj_clock, false).mMapNode = clock;
  bool clock_inv = (dff_cell->clock_sense() == 2);
  add_mapreq(sbj_clock, clock_inv);

  ymuint clear_sense = dff_cell->clear_sense();
  if ( clear_sense != 0 ) {
    const SbjNode* sbj_clear = inv ? sbj_dff->preset() : sbj_dff->clear();
    BnNode* clear = dff->_clear();
    ASSERT_COND( clear != nullptr );
    get_node_info(sbj_clear, false).mMapNode = clear;
    bool clear_inv = (clear_sense == 2);
    // sbj_clear->output_fanin() == nullptr の時もうまくいく．
    add_mapreq(sbj_clear, clear_inv);
  }

  ymuint preset_sense = dff_cell->preset_sense();
  if ( preset_sense != 0 ) {
    const SbjNode* sbj_preset = inv ? sbj_dff->clear() : sbj_dff->preset();
    BnNode* preset = dff->_preset();
    ASSERT_COND( preset != nullptr );
    get_node_info(sbj_preset, false).mMapNode = preset;
    bool preset_inv = (preset_sense == 2);
    // sbj_preset->output_fanin() == nullptr の時もうまくいく．
    add_mapreq(sbj_preset, preset_inv);
  }
}

// @brief ラッチの生成を行う．
void
MapGen::gen_latch(const BdnLatch* sbj_latch)
{
}

// @brief マッピング要求を追加する．
void
MapGen::add_mapreq(const BdnNode* node,
		   bool inv)
{
  mMapReqList.push_back(MapReq(node, inv));
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
ymuint
MapGen::back_trace(const SbjNode* node,
		   bool inv)
{
  NodeInfo& node_info = get_node_info(node, inv);
  ymuint mapnode = node_info.mMapNode;
  if ( mapnode != kBnNullId ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  // node を根とするマッチを取り出す．
  const Cut& match = node_info.mMatch;
  ymuint ni = match.leaf_num();

  // 生成されたファンインのBnNodeを tmp_fanins に格納する．
  // ちょっと考えればわかるけど tmp_fanins を上の for
  // ループで使わないのは再帰呼び出しで多重に生成されることになるから．
  vector<BnNode*> tmp_fanins(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    NodeInfo& inode_info = get_node_info(inode, iinv);
    BnNode* imapnode = inode_info.mMapNode;
    tmp_fanins[i] = imapnode;
  }

  // 新しいノードを作り mNodeMap に登録する．
  mapnode = mMapGraph->new_logic(tmp_fanins, node_info.mCell);
  node_info.mMapNode = mapnode;

  return mapnode;
}

END_NAMESPACE_YM_CELLMAP
