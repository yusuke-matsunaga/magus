
/// @file MapGen.cc
/// @brief MapGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MapGen.h"
#include "MapRecord.h"
#include "Cut.h"

#include "ym/BnBuilder.h"
#include "ym/TvFunc.h"

#include "SbjNode.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjPort.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
MapGen::MapGen()
{
}

// デストラクタ
MapGen::~MapGen()
{
}

// @brief マッピング結果を BnBuilder にセットする．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] record マッピング結果
// @param[out] mapgraph マッピング結果を格納するネットワーク
// @param[out] lut_num LUT数
// @param[out] depth 最大段数
void
MapGen::generate(const SbjGraph& sbjgraph,
		 const MapRecord& record,
		 BnBuilder& mapgraph,
		 ymuint& lut_num,
		 ymuint& depth)
{
  mapgraph.clear();

  mapgraph.set_model_name(sbjgraph.name());

  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id());

  // 外部入力の生成
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    ymuint map_id = node->id() * 2;
    ymuint node_id = mapgraph.add_input(string());
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = node_id;
    node_info.mDepth = 0;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  ymuint no = sbjgraph.output_num();
  int max_depth = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    ymuint node_id;
    if ( node ) {
      node_id = back_trace(node, inv, record, mapgraph);
      int depth1 = mNodeInfo[node->id()].mDepth;
      if ( max_depth < depth1 ) {
	max_depth = depth1;
      }
    }
    else {
      TvFunc tv;
      if ( inv ) {
	tv = TvFunc::const_one(0);
      }
      else {
	tv = TvFunc::const_zero(0);
      }
      node_id = mapgraph.add_tv(string(), tv);
    }
    ymuint onode_id = mapgraph.add_output(string(), node_id);
    mNodeInfo[onode->id()].mMapNode[0] = onode_id;
  }

  // DFFの生成
  ymuint nf = sbjgraph.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = sbjgraph.dff(i);
    BnBuilder::DffInfo& dff_info = mapgraph.add_dff(string());

    const SbjNode* input = dff->data_input();
    dff_info.mInput = mNodeInfo[input->id()].mMapNode[0];

    const SbjNode* output = dff->data_output();
    dff_info.mOutput = mNodeInfo[output->id()].mMapNode[0];

    const SbjNode* clock = dff->clock();
    dff_info.mClock = mNodeInfo[clock->id()].mMapNode[0];

    const SbjNode* clear = dff->clear();
    if ( clear != nullptr ) {
      dff_info.mClear = mNodeInfo[clear->id()].mMapNode[0];
    }

    const SbjNode* preset = dff->preset();
    if ( preset != nullptr ) {
      dff_info.mPreset = mNodeInfo[preset->id()].mMapNode[0];
    }
  }

  // ラッチの生成
  ymuint nlatch = sbjgraph.latch_num();
  for (ymuint i = 0; i < nlatch; ++ i) {
    const SbjLatch* latch = sbjgraph.latch(i);
    BnBuilder::LatchInfo& latch_info = mapgraph.add_latch(string());

    const SbjNode* input = latch->data_input();
    latch_info.mInput = mNodeInfo[input->id()].mMapNode[0];

    const SbjNode* output = latch->data_output();
    latch_info.mOutput = mNodeInfo[output->id()].mMapNode[0];

    const SbjNode* enable = latch->enable();
    latch_info.mEnable = mNodeInfo[enable->id()].mMapNode[0];

    const SbjNode* clear = latch->clear();
    if ( clear != nullptr ) {
      latch_info.mClear = mNodeInfo[clear->id()].mMapNode[0];
    }

    const SbjNode* preset = latch->preset();
    if ( preset != nullptr ) {
      latch_info.mPreset = mNodeInfo[preset->id()].mMapNode[0];
    }
  }

  // ポートの生成
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* sbjport = sbjgraph.port(i);
    ymuint nb = sbjport->bit_width();
    vector<ymuint> tmp(nb);
    for (ymuint j = 0; j < nb; ++ j) {
      const SbjNode* sbjnode = sbjport->bit(j);
      tmp[j] = mNodeInfo[sbjnode->id()].mMapNode[0];
    }
    mapgraph.add_port(sbjport->name(), tmp);
  }

  bool stat = mapgraph.wrap_up();
  ASSERT_COND( stat );

  lut_num = mapgraph.logic_num();
  depth = max_depth;
}

// @brief 最終結果を作るためのバックトレースを行う．
// @param[in] node 対象のノード
// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
// @param[in] record マッピング結果
// @param[out] mapnetwork マッピング結果のネットワーク
ymuint
MapGen::back_trace(const SbjNode* node,
		   bool inv,
		   const MapRecord& record,
		   BnBuilder& mapnetwork)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  ymuint node_id = node_info.mMapNode[idx];
  if ( node_id ) {
    // すでに生成済みならそのノードを返す．
    return node_id;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    ASSERT_COND(inv );
    // NOT ゲートを表す LUT を作る．
    ymuint src_id = node_info.mMapNode[0];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    mapnetwork.connect(src_id, node_id, 0);
    node_info.mMapNode[1] = node_id;
    return node_id;
  }

  // node を根とするカットを取り出す．
  const Cut* cut = record.get_cut(node);
  if ( cut == nullptr ) {
    cout << "Error!: " << node->id_str()
	 << " has no cuts" << endl;
    ASSERT_NOT_REACHED;
  }

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    back_trace(inode, false, record, mapnetwork);
  }

  // カットの実現している関数の真理値表を得る．
  TvFunc tv = cut->make_tv(inv);

  // 新しいノードを作り mNodeMap に登録する．
  node_id = mapnetwork.add_tv(string(), tv);
  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    ymuint src_id = inode_info.mMapNode[0];
    mapnetwork.connect(src_id, node_id, i);
    int idepth1 = inode_info.mDepth;
    if ( idepth < idepth1 ) {
      idepth = idepth1;
    }
  }
  node_info.mMapNode[idx] = node_id;
  node_info.mDepth = idepth + 1;

  return node_id;
}

END_NAMESPACE_YM_LUTMAP
