
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


BEGIN_NAMESPACE_YM_LUTMAP

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
MapRecord::init(const SbjGraph& sbjgraph)
{
  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id());
}

// @brief カットの情報だけコピーする．
void
MapRecord::copy(const MapRecord& src)
{
  ymuint n = src.mNodeInfo.size();
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mCut = src.mNodeInfo[i].mCut;
  }
}

// @blif カットを記録する．
// @param[in] node 該当のノード
// @param[in] cut 対応するカット
void
MapRecord::set_cut(const SbjNode* node,
		   const Cut* cut)
{
  mNodeInfo[node->id()].mCut = cut;
}

// @brief カットを取り出す．
// @param[in] node 該当のノード
const Cut*
MapRecord::get_cut(const SbjNode* node)
{
  return mNodeInfo[node->id()].mCut;
}

// @brief マッピング結果を BnNetwork にセットする．
void
MapRecord::gen_mapgraph(const SbjGraph& sbjgraph,
			BnBuilder& mapgraph,
			ymuint& lut_num,
			ymuint& depth)
{
  mapgraph.clear();

  mapgraph.set_model_name(sbjgraph.name());

  mNextId = sbjgraph.max_node_id() * 2;

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
      node_id = back_trace(node, inv, mapgraph);
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

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
ymuint
MapRecord::back_trace(const SbjNode* node,
		      bool inv,
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
  const Cut* cut = node_info.mCut;

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    back_trace(inode, false, mapnetwork);
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

// @brief マッピング結果の LUT 数を見積もる．
int
MapRecord::estimate(const SbjGraph& sbjgraph)
{
  for (vector<NodeInfo>::iterator p = mNodeInfo.begin();
       p != mNodeInfo.end(); ++ p) {
    NodeInfo& node_info = *p;
    node_info.mMapCount[0] = 0;
    node_info.mMapCount[1] = 0;
    node_info.mCovCount = 0;
    node_info.mTmpFlag = false;
  }

  // 外部入力の生成
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapCount[0] = 1;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  int lut_num = 0;
  ymuint no = sbjgraph.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    if ( node && node->is_logic() ) {
      if ( mNodeInfo[node->id()].mCut == nullptr ) {
	lut_num = -1;
	break;
      }
      bool inv = onode->output_fanin_inv();
      lut_num += back_trace2(node, inv);
    }
  }
  return lut_num;
}

// estimate 用のバックトレース
int
MapRecord::back_trace2(const SbjNode* node,
		       bool inv)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  ++ node_info.mMapCount[idx];
  if ( node_info.mMapCount[idx] > 1 ) {
    return 0;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    node_info.mMapCount[1] = 1;
    // インバーターが必要ということ
    return 1;
  }

  // node を根とするカットを取り出す．
  const Cut* cut = node_info.mCut;

  // cut のカバーしているノードの mCovCount を1つ増やす．
  for (ymuint i = 0; i < cut->input_num(); ++ i) {
    const SbjNode* leaf = cut->input(i);
    mNodeInfo[leaf->id()].mTmpFlag = true;
  }
  mark_cover(node, cut);
  clear_mark(node);

  int lut_num = 1;
  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut->input(i);
    lut_num += back_trace2(inode, false);
  }

  return lut_num;
}

// @brief 直前の estimate の結果 node が fanout node なら true を返す．
bool
MapRecord::check_fonode(const SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  return (node_info.mMapCount[0] + node_info.mMapCount[1]) > 1;
}

// @brief 直前の estimate の結果で node のカバーされている回数を返す．
int
MapRecord::cover_count(const SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  return node_info.mCovCount;
}

// cut でカバーされるノードの mCovCount を一つ増やす．
void
MapRecord::mark_cover(const SbjNode* node,
		      const Cut* cut)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( !node_info.mTmpFlag ) {
    node_info.mTmpFlag = true;
    ++ node_info.mCovCount;
    mark_cover(node->fanin(0), cut);
    mark_cover(node->fanin(1), cut);
  }
}

// mark_cover でつけた mTmpFlag を下ろす．
void
MapRecord::clear_mark(const SbjNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mTmpFlag ) {
    node_info.mTmpFlag = false;
    if ( node->is_logic() ) {
      clear_mark(node->fanin(0));
      clear_mark(node->fanin(1));
    }
  }
}

// @brief 新しいノード番号を得る．
ymuint
MapRecord::new_id()
{
  ymuint id = mNextId;
  ++ mNextId;
  return id;
}

END_NAMESPACE_YM_LUTMAP
