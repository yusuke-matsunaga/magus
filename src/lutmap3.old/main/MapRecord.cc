
/// @file MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"
#include "Cut.h"
#include "ym/BnBuilder.h"
#include "ym/TvFunc.h"
#include "SbjNode.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjPort.h"


BEGIN_NAMESPACE_YM_LUTMAP3

// コンストラクタ
MapRecord::MapRecord()
{
}

// @brief コピーコンストラクタ
MapRecord::MapRecord(const MapRecord& src) :
  mCutArray(src.mCutArray)
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
// @param[in] sbjgraph サブジェクトグラフ
void
MapRecord::init(const SbjGraph& sbjgraph)
{
  mCutArray.clear();
  mCutArray.resize(sbjgraph.max_node_id(), nullptr);
}

// @blif カットを記録する．
// @param[in] node 該当のノード
// @param[in] cut 対応するカット
void
MapRecord::set_cut(const SbjNode* node,
		   const Cut* cut)
{
  mCutArray[node->id()] = cut;
}

// @brief カットを取り出す．
// @param[in] node 該当のノード
const Cut*
MapRecord::get_cut(const SbjNode* node) const
{
  return mCutArray[node->id()];
}

// @brief マッピング結果を BnNetwork にセットする．
void
MapRecord::gen_mapgraph(const SbjGraph& sbjgraph,
			BnBuilder& mapgraph,
			ymuint& lut_num,
			ymuint& depth) const
{
  mapgraph.clear();

  mapgraph.set_model_name(sbjgraph.name());

  ymuint n = sbjgraph.max_node_id();
  vector<MapWork> work(n);

  // 外部入力の生成
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    ymuint node_id = mapgraph.add_input(string());
    MapWork& map_work = work[node->id()];
    map_work.mMapNode[0] = node_id;
    map_work.mDepth = 0;
  }

  // 外部出力で要求されている極性を記録しておく．
  ymuint no = sbjgraph.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    ymuint phase = 0;
    if ( onode->output_fanin_inv() ) {
      phase = 2;
    }
    else {
      phase = 1;
    }
    work[node->id()].mReqPhase |= phase;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  int max_depth = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    ymuint node_id;
    if ( node ) {
      node_id = back_trace(node, inv, work, mapgraph);
      int depth1 = work[node->id()].mDepth;
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
    work[onode->id()].mMapNode[0] = onode_id;
  }

  // DFFの生成
  ymuint nf = sbjgraph.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = sbjgraph.dff(i);
    BnBuilder::DffInfo& dff_info = mapgraph.add_dff(string());

    const SbjNode* input = dff->data_input();
    dff_info.mInput = work[input->id()].mMapNode[0];

    const SbjNode* output = dff->data_output();
    dff_info.mOutput = work[output->id()].mMapNode[0];

    const SbjNode* clock = dff->clock();
    dff_info.mClock = work[clock->id()].mMapNode[0];

    const SbjNode* clear = dff->clear();
    if ( clear != nullptr ) {
      dff_info.mClear = work[clear->id()].mMapNode[0];
    }

    const SbjNode* preset = dff->preset();
    if ( preset != nullptr ) {
      dff_info.mPreset = work[preset->id()].mMapNode[0];
    }
  }

  // ラッチの生成
  ymuint nlatch = sbjgraph.latch_num();
  for (ymuint i = 0; i < nlatch; ++ i) {
    const SbjLatch* latch = sbjgraph.latch(i);
    BnBuilder::LatchInfo& latch_info = mapgraph.add_latch(string());

    const SbjNode* input = latch->data_input();
    latch_info.mInput = work[input->id()].mMapNode[0];

    const SbjNode* output = latch->data_output();
    latch_info.mOutput = work[output->id()].mMapNode[0];

    const SbjNode* enable = latch->enable();
    latch_info.mEnable = work[enable->id()].mMapNode[0];

    const SbjNode* clear = latch->clear();
    if ( clear != nullptr ) {
      latch_info.mClear = work[clear->id()].mMapNode[0];
    }

    const SbjNode* preset = latch->preset();
    if ( preset != nullptr ) {
      latch_info.mPreset = work[preset->id()].mMapNode[0];
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
      tmp[j] = work[sbjnode->id()].mMapNode[0];
    }
    mapgraph.add_port(sbjport->name(), tmp);
  }

  bool stat = mapgraph.wrap_up();
  ASSERT_COND( stat );

  lut_num = mapgraph.logic_num();
  depth = max_depth;
}

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
ymuint
MapRecord::back_trace(const SbjNode* node,
		      bool inv,
		      vector<MapWork>& work,
		      BnBuilder& mapnetwork) const
{
  MapWork& map_work = work[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  ymuint node_id = map_work.mMapNode[idx];
  if ( node_id ) {
    // すでに生成済みならそのノードを返す．
    return node_id;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    ASSERT_COND(inv );
    // NOT ゲートを表す LUT を作る．
    ymuint src_id = map_work.mMapNode[0];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    mapnetwork.connect(src_id, node_id, 0);
    map_work.mMapNode[1] = node_id;
    return node_id;
  }

  // 反対側の極性のノードができていたら NOT ゲートを作る．
  if ( map_work.mMapNode[idx ^ 1] ) {
    // NOT ゲートを表す LUT を作る．
    ymuint src_id = map_work.mMapNode[idx ^ 1];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    mapnetwork.connect(src_id, node_id, 0);
    map_work.mMapNode[idx] = node_id;
    return node_id;
  }

  // node を根とするカットを取り出す．
  const Cut* cut = mCutArray[node->id()];

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  vector<bool> input_inv(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    bool inv = false;
    if ( work[inode->id()].mReqPhase == 2 ) {
      inv = true;
    }
    input_inv[i] = inv;
    back_trace(inode, inv, work, mapnetwork);
  }

  // カットの実現している関数の真理値表を得る．
  TvFunc tv = cut->make_tv(inv, input_inv);

  // 新しいノードを作り mNodeMap に登録する．
  node_id = mapnetwork.add_tv(string(), tv);
  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    MapWork& imap_work = work[inode->id()];
    ymuint idx = input_inv[i] ? 1 : 0;
    ymuint src_id = imap_work.mMapNode[idx];
    mapnetwork.connect(src_id, node_id, i);
    int idepth1 = imap_work.mDepth;
    if ( idepth < idepth1 ) {
      idepth = idepth1;
    }
  }
  map_work.mMapNode[idx] = node_id;
  map_work.mDepth = idepth + 1;

  return node_id;
}

END_NAMESPACE_YM_LUTMAP3
