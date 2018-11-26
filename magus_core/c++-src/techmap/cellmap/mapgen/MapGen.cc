
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
#include "ym/ClibCell.h"
#include "ym/ClibCellPin.h"
#include "ym/Expr.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_CELLMAP

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
  int np = sbjgraph.port_num();
  for ( int i = 0; i < np; ++ i ) {
    const SbjPort* sbjport = sbjgraph.port(i);
    gen_port(sbjport);
  }

  // D-FF の生成
  int n_dff = sbjgraph.dff_num();
  for ( int i = 0; i < n_dff; ++ i ) {
    const SbjDff* sbj_dff = sbjgraph.dff(i);
    gen_dff(sbj_dff, record);
  }

  // ラッチの生成
  int n_latch = sbjgraph.latch_num();
  for ( int i = 0; i < n_latch; ++ i ) {
    const SbjLatch* sbj_latch = sbjgraph.latch(i);
    gen_latch(sbj_latch);
  }

  // 組み合わせ回路の出力からバックトレースを行い全ノードの生成を行う．
  for ( const auto& rec: mMapReqList ) {
    const SbjNode* onode = rec.mNode;
    bool ext_inv = rec.mInv;
    const SbjNode* node = onode->fanin(0);
    bool inv = onode->output_fanin_inv() ^ ext_inv;
    int mapnode = 0;
    if ( node ) {
      mapnode = back_trace(node, inv, record);
    }
    else {
      int node_id = 0;
      if ( inv ) {
	// 定数1ノードを作る．
	int const1_cell = record.const1_cell();
	ASSERT_COND( const1_cell != -1 );
	mapnode = mMapGraph->new_logic(string(), const1_cell);
      }
      else {
	// 定数0ノードを作る．
	int const0_cell = record.const0_cell();
	ASSERT_COND( const0_cell != -1 );
	mapnode = mMapGraph->new_logic(string(), const0_cell);
      }
    }
    int omapnode = node_info(onode, false).mMapNode;
    mMapGraph->connect(mapnode, omapnode, 0);
  }
}

// @brief ポートの生成を行う．
void
MapGen::gen_port(const SbjPort* sbj_port)
{
  int nb = sbj_port->bit_width();
  vector<int> iovect(nb);
  for ( int i = 0; i < nb; ++ i ) {
    const SbjNode* sbj_node = sbj_port->bit(i);
    if ( sbj_node->is_input() ) {
      iovect[i] = 0;
    }
    else {
      iovect[i] = 1;
    }
  }
  int port_id = mMapGraph->new_port(sbj_port->name(), iovect);
  auto& dst_port = mMapGraph->port(port_id);
  for ( int j = 0; j < nb; ++ j ) {
    const SbjNode* sbj_node = sbj_port->bit(j);
    auto& node = mMapGraph->node(dst_port.bit(j));
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
MapGen::gen_dff(const SbjDff* sbj_dff,
		const MapRecord& record)
{
  int dff_cell0_id = record.get_dff_cell(sbj_dff, false);
  int dff_cell1_id = record.get_dff_cell(sbj_dff, true);
  int cell_id = -1;
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
  ClibFFInfo ff_info = cell.ff_info();
  int dff_id = mMapGraph->new_dff(string(), cell_id);
  auto& dff = mMapGraph->dff(dff_id);

  const SbjNode* sbj_output = sbj_dff->data_output();
  int output1 = dff.output();
  ASSERT_COND( output1 != kBnNullId );
  node_info(sbj_output, inv).mMapNode = output1;
  int output2 = dff.xoutput();
  if ( output2 != kBnNullId ) {
    node_info(sbj_output, !inv).mMapNode = output2;
  }

  const SbjNode* sbj_input = sbj_dff->data_input();
  int input = dff.input();
  node_info(sbj_input, false).mMapNode = input;
  add_mapreq(sbj_input, inv);

  const SbjNode* sbj_clock = sbj_dff->clock();
  int clock = dff.clock();
  node_info(sbj_clock, false).mMapNode = clock;
  bool clock_inv = (ff_info.clock_sense() == 2);
  add_mapreq(sbj_clock, clock_inv);

  int clear_sense = ff_info.clear_sense();
  if ( clear_sense != 0 ) {
    const SbjNode* sbj_clear = inv ? sbj_dff->preset() : sbj_dff->clear();
    int clear = dff.clear();
    ASSERT_COND( clear != kBnNullId );
    node_info(sbj_clear, false).mMapNode = clear;
    bool clear_inv = (clear_sense == 2);
    // sbj_clear->output_fanin() == nullptr の時もうまくいく．
    add_mapreq(sbj_clear, clear_inv);
  }

  int preset_sense = ff_info.preset_sense();
  if ( preset_sense != 0 ) {
    const SbjNode* sbj_preset = inv ? sbj_dff->clear() : sbj_dff->preset();
    int preset = dff.preset();
    ASSERT_COND( preset != kBnNullId );
    node_info(sbj_preset, false).mMapNode = preset;
    bool preset_inv = (preset_sense == 2);
    // sbj_preset->output_fanin() == nullptr の時もうまくいく．
    add_mapreq(sbj_preset, preset_inv);
  }
}

// @brief ラッチの生成を行う．
void
MapGen::gen_latch(const SbjLatch* sbj_latch)
{
#warning "未完";
}

// @brief マッピング要求を追加する．
void
MapGen::add_mapreq(const SbjNode* sbj_node,
		   bool inv)
{
  mMapReqList.push_back(MapReq(sbj_node, inv));
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
int
MapGen::back_trace(const SbjNode* node,
		   bool inv,
		   const MapRecord& record)
{
  NodeInfo& node_info = this->node_info(node, inv);
  int mapnode = node_info.mMapNode;
  if ( mapnode != kBnNullId ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  // node を根とするマッチを取り出す．
  const Cut& match = record.get_node_match(node, inv);
  int cell_id = record.get_node_cell(node, inv);

  // 新しいノードを作り mNodeMap に登録する．
  mapnode = mMapGraph->new_logic(string(), cell_id);
  node_info.mMapNode = mapnode;

  int ni = match.leaf_num();
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* inode = match.leaf_node(i);
    bool iinv = match.leaf_inv(i);
    int iid = back_trace(inode, iinv, record);
    mMapGraph->connect(iid, mapnode, i);
  }

  return mapnode;
}

// @brief node に関係する情報を得る．
MapGen::NodeInfo&
MapGen::node_info(const SbjNode* node,
		  bool inv)
{
  return mNodeInfo[node->id() * 2 + inv];
}

END_NAMESPACE_CELLMAP
