
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

#define LUTMAP_DEBUG_MAPGEN 0


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
#if LUTMAP_DEBUG_MAPGEN
  cout << "MapGen::generate() start" << endl;
#endif

  mapgraph.clear();

  mapgraph.set_model_name(sbjgraph.name());

  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id());

  ymuint ni = sbjgraph.input_num();
  ymuint no = sbjgraph.output_num();

  // 外部入力の生成
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    ymuint map_id = node->id() * 2;
    ymuint node_id = mapgraph.add_input(string());
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = node_id;
    node_info.mDepth = 0;
  }

  // 外部出力から要求されている極性を記録する．
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    ymuint phase = onode->output_fanin_inv() ? 2 : 1;
    mNodeInfo[node->id()].mReqPhase = phase;
#if LUTMAP_DEBUG_MAPGEN
    cout << node->id_str() << ": reqphase = " << phase << endl;
#endif
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  int max_depth = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    ymuint node_id;
    if ( node ) {
      node_id = gen_back_trace(node, inv, record, mapgraph);
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
#if LUTMAP_DEBUG_MAPGEN
      cout << onode->id_str() << endl;
#endif
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

#if LUTMAP_DEBUG_MAPGEN
  cout << "MapGen::generate() end" << endl;
#endif
}

// @brief 最終結果を作るためのバックトレースを行う．
// @param[in] node 対象のノード
// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
// @param[in] record マッピング結果
// @param[out] mapnetwork マッピング結果のネットワーク
ymuint
MapGen::gen_back_trace(const SbjNode* node,
		       bool output_inv,
		       const MapRecord& record,
		       BnBuilder& mapnetwork)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "back_trace(" << node->id_str() << ", " << output_inv << ")" << endl;
#endif
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (output_inv) ? 1 : 0;
  ymuint node_id = node_info.mMapNode[idx];
  if ( node_id ) {
    // すでに生成済みならそのノードを返す．
    return node_id;
  }

  if ( node->is_input() ) {
    // ということは output_inv = true のはず．
    ASSERT_COND( output_inv );
    // NOT ゲートを表す LUT を作る．
    ymuint src_id = node_info.mMapNode[0];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    mapnetwork.connect(src_id, node_id, 0);
    node_info.mMapNode[1] = node_id;
#if LUTMAP_DEBUG_MAPGEN
    cout << node->id_str() << "[1]" << endl;
#endif
    return node_id;
  }

  if ( node_info.mMapNode[idx ^ 1] ) {
    // 逆の極性のノードが生成済みなら NOT ゲートを作る．
    ymuint src_id = node_info.mMapNode[idx ^ 1];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    mapnetwork.connect(src_id, node_id, 0);
    node_info.mMapNode[idx] = node_id;
#if LUTMAP_DEBUG_MAPGEN
    cout << node->id_str() << "[" << (idx ^ 1) << "]" << endl;
#endif
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
  vector<bool> input_inv(ni, false);
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    input_inv[i] = (mNodeInfo[inode->id()].mReqPhase == 2) ? true : false;
    gen_back_trace(inode, input_inv[i], record, mapnetwork);
  }

  // カットの実現している関数の真理値表を得る．
  TvFunc tv = cut->make_tv(output_inv, input_inv);

  // 新しいノードを作り mNodeMap に登録する．
  node_id = mapnetwork.add_tv(string(), tv);
#if LUTMAP_DEBUG_MAPGEN
  cout << node->id_str() << "[" << idx << "]" << endl;
#endif

  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    ymuint idx = input_inv[i] ? 1 : 0;
    ymuint src_id = inode_info.mMapNode[idx];
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

// @brief マッピング結果から見積もりを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] record マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 最大段数
void
MapGen::estimate(const SbjGraph& sbjgraph,
		 const MapRecord& record,
		 ymuint& lut_num,
		 ymuint& depth)
{
#if LUTMAP_DEBUG_MAPEST
  cout << "MapGen::estimate() start" << endl;
#endif

  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id());

  mFanoutPointList.clear();

  ymuint ni = sbjgraph.input_num();
  ymuint no = sbjgraph.output_num();

  // 外部入力の生成
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = 1;
  }

  // 外部出力から要求されている極性を記録する．
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    ymuint phase = onode->output_fanin_inv() ? 2 : 1;
    mNodeInfo[node->id()].mReqPhase = phase;
#if LUTMAP_DEBUG_MAPEST
    cout << node->id_str() << ": reqphase = " << phase << endl;
#endif
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  lut_num = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    if ( node ) {
      lut_num += est_back_trace(node, inv, record);
    }
    else {
      lut_num += 1;
#if LUTMAP_DEBUG_MAPEST
      cout << onode->id_str() << endl;
#endif
    }
  }

#if LUTMAP_DEBUG_MAPEST
  cout << "MapGen::estimate() end" << endl;
#endif
}

// @brief 最終結果を作るためのバックトレースを行う．
// @param[in] node 対象のノード
// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
// @param[in] record マッピング結果
ymuint
MapGen::est_back_trace(const SbjNode* node,
		       bool inv,
		       const MapRecord& record)
{
#if LUTMAP_DEBUG_MAPEST
  cout << "back_trace(" << node->id_str() << ", " << inv << ")" << endl;
#endif
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  ++ node_info.mMapNode[idx];
  if ( node_info.mMapNode[idx] > 1 ) {
    if ( node_info.mMapNode[idx] == 2 ) {
      mFanoutPointList.push_back(node);
    }
    return 0;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    // インバーターが必要ということ
#if LUTMAP_DEBUG_MAPEST
    cout << node->id_str() << "[1]" << endl;
#endif
    return 1;
  }

  if ( node_info.mMapNode[idx ^ 1] ) {
    // 逆の極性のノードが生成済み
#if LUTMAP_DEBUG_MAPEST
    cout << node->id_str() << "[" << (idx ^ 1) << "]" << endl;
#endif
    return 1;
  }

  // node を根とするカットを取り出す．
  const Cut* cut = record.get_cut(node);

#if LUTMAP_DEBUG_MAPEST
    cout << node->id_str() << "[" << idx << "]" << endl;
#endif

  int lut_num = 1;
  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    bool iinv = (mNodeInfo[inode->id()].mReqPhase == 2) ? true : false;
    lut_num += est_back_trace(inode, iinv, record);
  }

  return lut_num;
}

END_NAMESPACE_YM_LUTMAP
