
/// @file MapGen.cc
/// @brief MapGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.

#include "MapGen.h"
#include "MapRecord.h"
#include "SbjPort.h"
#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnDff.h"
#include "ym/BnNode.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"
#include "ym/ClibPin.h"
#include "ym/Expr.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_CELLMAP

BEGIN_NONAMESPACE

inline
ymuint
encode(
  ymuint pos,
  ymuint sense
)
{
  return pos | (sense << 3);
}

END_NONAMESPACE

// @brief マッピング結果を BnNetwork にセットする．
void
MapGen::generate(
  const SbjGraph& sbjgraph,
  const MapRecord& record
)
{
  clear();
  set_name(sbjgraph.name());

  mMapReqList.clear();
  mMapReqList.reserve(sbjgraph.output_num());

  // ポートの生成
  SizeType np = sbjgraph.port_num();
  for ( SizeType i = 0; i < np; ++ i ) {
    const SbjPort* sbjport = sbjgraph.port(i);
    gen_port(sbjport);
  }

  // D-FF の生成
  SizeType n_dff = sbjgraph.dff_num();
  for ( SizeType i = 0; i < n_dff; ++ i ) {
    const SbjDff* sbj_dff = sbjgraph.dff(i);
    gen_dff(sbj_dff, record);
  }

  // ラッチの生成
  SizeType n_latch = sbjgraph.latch_num();
  for ( SizeType i = 0; i < n_latch; ++ i ) {
    const SbjLatch* sbj_latch = sbjgraph.latch(i);
    gen_latch(sbj_latch);
  }

  // 組み合わせ回路の出力からバックトレースを行い全ノードの生成を行う．
  for ( const auto& rec: mMapReqList ) {
    const SbjNode* onode = rec.mNode;
    bool ext_inv = rec.mInv;
    const SbjNode* node = onode->fanin(0);
    bool inv = onode->output_fanin_inv() ^ ext_inv;
    SizeType mapnode = 0;
    if ( node ) {
      mapnode = back_trace(node, inv, record);
    }
    else {
      SizeType node_id = 0;
      if ( inv ) {
	// 定数1ノードを作る．
	SizeType const1_cell = record.const1_cell();
	ASSERT_COND( const1_cell != CLIB_NULLID );
	mapnode = new_logic_cell({}, const1_cell, {});
      }
      else {
	// 定数0ノードを作る．
	SizeType const0_cell = record.const0_cell();
	ASSERT_COND( const0_cell != CLIB_NULLID );
	mapnode = new_logic_cell({}, const0_cell, {});
      }
    }
    SizeType omapnode = node_info(onode, false).mMapNode;
    set_output_src(omapnode, mapnode);
  }
}

// @brief ポートの生成を行う．
void
MapGen::gen_port(
  const SbjPort* sbj_port
)
{
  SizeType nb = sbj_port->bit_width();
  vector<BnDir> iovect(nb);
  for ( SizeType i = 0; i < nb; ++ i ) {
    const SbjNode* sbj_node = sbj_port->bit(i);
    if ( sbj_node->is_input() ) {
      iovect[i] = BnDir::INPUT;
    }
    else {
      iovect[i] = BnDir::OUTPUT;
    }
  }
  SizeType port_id = new_port(sbj_port->name(), iovect);
  auto& dst_port = port(port_id);
  for ( SizeType j = 0; j < nb; ++ j ) {
    const SbjNode* sbj_node = sbj_port->bit(j);
    auto& node = this->node(dst_port.bit(j));
    if ( sbj_node->is_input() ) {
      ASSERT_COND( node.is_input() );
      node_info(sbj_node, false).mMapNode = node.id();
    }
    else if ( sbj_node->is_output() ) {
      ASSERT_COND( node.is_output() );
      node_info(sbj_node, false).mMapNode = node.id();
      add_mapreq(sbj_node, false);
    }
  }
}

// @brief D-FF の生成を行う．
void
MapGen::gen_dff(
  const SbjDff* sbj_dff,
  const MapRecord& record
)
{
  int dff_cell0_id = record.get_dff_cell(sbj_dff, false);
  int dff_cell1_id = record.get_dff_cell(sbj_dff, true);
  SizeType cell_id = -1;
  bool inv = false;
  if ( dff_cell0_id == -1 ) {
    if ( dff_cell1_id == -1 ) {
      ASSERT_NOT_REACHED;
    }
    cell_id = dff_cell1_id;
    inv = true;
  }
  else {
    cell_id = dff_cell0_id;
  }
  const ClibCell& cell = record.cell_library().cell(cell_id);
  //ClibFFInfo ff_info = cell.ff_info();
  int dff_id = new_dff(string(), cell_id);
  auto& dff = this->dff(dff_id);

  const SbjNode* sbj_output = sbj_dff->data_output();
  SizeType output1 = dff.data_out();
  ASSERT_COND( output1 != BNET_NULLID );
  node_info(sbj_output, inv).mMapNode = output1;
#if 0
  int output2 = dff.xoutput();
  if ( output2 != BNET_NULLID ) {
    node_info(sbj_output, !inv).mMapNode = output2;
  }
#endif

#warning "TODO: type() 別のコード"
  const SbjNode* sbj_input = sbj_dff->data_input();
  SizeType input = dff.data_in();
  node_info(sbj_input, false).mMapNode = input;
  add_mapreq(sbj_input, inv);

  const SbjNode* sbj_clock = sbj_dff->clock();
  int clock = dff.clock();
  node_info(sbj_clock, false).mMapNode = clock;
  bool clock_inv = false; /*(ff_info.clock_sense() == 2);*/
  add_mapreq(sbj_clock, clock_inv);

#if 0
  int clear_sense = ff_info.clear_sense();
  if ( clear_sense != 0 ) {
    const SbjNode* sbj_clear = inv ? sbj_dff->preset() : sbj_dff->clear();
    int clear = dff.clear();
    ASSERT_COND( clear != BNET_NULLID );
    node_info(sbj_clear, false).mMapNode = clear;
    bool clear_inv = (clear_sense == 2);
    // sbj_clear->output_fanin() == nullptr の時もうまくいく．
    add_mapreq(sbj_clear, clear_inv);
  }

  int preset_sense = ff_info.preset_sense();
  if ( preset_sense != 0 ) {
    const SbjNode* sbj_preset = inv ? sbj_dff->clear() : sbj_dff->preset();
    int preset = dff.preset();
    ASSERT_COND( preset != BNET_NULLID );
    node_info(sbj_preset, false).mMapNode = preset;
    bool preset_inv = (preset_sense == 2);
    // sbj_preset->output_fanin() == nullptr の時もうまくいく．
    add_mapreq(sbj_preset, preset_inv);
  }
#endif
}

// @brief ラッチの生成を行う．
void
MapGen::gen_latch(
  const SbjLatch* sbj_latch
)
{
#warning "未完";
}

// @brief マッピング要求を追加する．
void
MapGen::add_mapreq(
  const SbjNode* sbj_node,
  bool inv
)
{
  mMapReqList.push_back(MapReq(sbj_node, inv));
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
int
MapGen::back_trace(
  const SbjNode* node,
  bool inv,
  const MapRecord& record
)
{
  NodeInfo& node_info = this->node_info(node, inv);
  int mapnode = node_info.mMapNode;
  if ( mapnode != BNET_NULLID ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  // node を根とするマッチを取り出す．
  const Cut& match = record.get_node_match(node, inv);
  SizeType cell_id = record.get_node_cell(node, inv);

  // 新しいノードを作り mNodeMap に登録する．
  SizeType ni = match.leaf_num();
  vector<SizeType> fanin_id_list(ni);
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    SizeType iid = back_trace(inode, iinv, record);
    fanin_id_list[i] = iid;
  }
  mapnode = new_logic_cell({}, cell_id, fanin_id_list);
  node_info.mMapNode = mapnode;

  return mapnode;
}

// @brief node に関係する情報を得る．
MapGen::NodeInfo&
MapGen::node_info(
  const SbjNode* node,
  bool inv
)
{
  return mNodeInfo[node->id() * 2 + inv];
}

END_NAMESPACE_CELLMAP
