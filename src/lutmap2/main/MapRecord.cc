
/// @file MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"
#include "ym/BnBuilder.h"
#include "ym/TvFunc.h"

#include "SbjGraph.h"
#include "SbjNode.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjPort.h"


BEGIN_NAMESPACE_YM_LUTMAP2

BEGIN_NONAMESPACE

// valmap に終端のノード番号をキーとしてビットベクタ値を登録する．
// その時の node の値を計算する．
ymuint64
eval_node(const SbjNode* node,
	  HashMap<ymuint, ymuint64>& valmap)
{
  if ( node == nullptr ) {
    return 0ULL;
  }

  // まずすでに評価済みかどうか調べる．
  // 葉のノードの場合もここに登録されている．
  ymuint64 ans;
  if ( !valmap.find(node->id(), ans) ) {
    // 未登録の場合は必ず論理ノード
    ASSERT_COND( node->is_logic() );

    // 子供の値を評価する．
    ymuint64 val0 = eval_node(node->fanin(0), valmap);
    if ( node->fanin_inv(0) ) {
      val0 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    ymuint64 val1 = eval_node(node->fanin(1), valmap);
    if ( node->fanin_inv(0) ) {
      val1 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    // 自分の値を計算する．
    if ( node->is_xor() ) {
      ans = val0 ^ val1;
    }
    else {
      ans = val0 & val1;
    }

    // 登録しておく．
    valmap.add(node->id(), ans);
  }

  return ans;
}

// カットの表している論理関数を評価する．
ymuint64
eval_cut(const SbjNode* root,
	 const vector<const SbjNode*>& cut_inputs,
	 const vector<ymuint64>& vals)
{
  ymuint ni = cut_inputs.size();
  ASSERT_COND( ni == vals.size() );
  HashMap<ymuint, ymuint64> valmap;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut_inputs[i];
    valmap.add(inode->id(), vals[i]);
  }
  return eval_node(root, valmap);
}

// @brief 論理関数を表す真理値表を得る．
TvFunc
make_tv(const SbjNode* root,
	const vector<const SbjNode*>& cut_inputs,
	bool inv,
	const vector<bool>& input_inv)
{
  ymuint ni = cut_inputs.size();
  ymuint np = 1 << ni;

  vector<int> tv(np);

  // 1 の値と 0 の値
  int v1 = inv ? 0 : 1;
  int v0 = inv ? 1 : 0;

  // 真理値表の各変数の値を表すビットベクタ
  // 6入力以上の場合には1語に収まらないので複数回にわけて処理する．
  vector<ymuint64> vals(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vals[i] = 0ULL;
  }

  ymuint64 s = 1ULL;
  ymuint p0 = 0;
  for (ymuint p = 0; p < np; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( input_inv[i] ) {
	if ( (p & (1U << i)) == 0U ) {
	  vals[i] |= s;
	}
      }
      else {
	if ( p & (1U << i) ) {
	  vals[i] |= s;
	}
      }
    }
    s <<= 1;
    if ( s == 0ULL ) {
      // 64 パタン目
      ymuint64 tmp = eval_cut(root, cut_inputs, vals);
      for (ymuint p1 = p0; p1 < p; ++ p1) {
	if ( tmp & (1ULL << (p1 - p0)) ) {
	  tv[p1] = v1;
	}
	else {
	  tv[p1] = v0;
	}
      }
      s = 1ULL;
      p0 = p + 1;
      for (ymuint i = 0; i < ni; ++ i) {
	vals[i] = 0ULL;
      }
    }
  }
  if ( s != 1ULL ) {
    // 処理されていない残りがあった．
    ymuint64 tmp = eval_cut(root, cut_inputs, vals);
    for (ymuint p1 = p0; p1 < np; ++ p1) {
      if ( tmp & (1ULL << (p1 - p0)) ) {
	tv[p1] = v1;
      }
      else {
	tv[p1] = v0;
      }
    }
  }

  return TvFunc(ni, tv);
}

END_NONAMESPACE

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

// @blif カットを記録する．
// @param[in] node 該当のノード
// @param[in] input_list 入力ノードのリスト
void
MapRecord::set_cut(const SbjNode* node,
		   const vector<const SbjNode*>& input_list)
{
  mNodeInfo[node->id()].mInputs = input_list;
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

  // まず外部出力で要求されている極性を記録しておく．
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
    mNodeInfo[node->id()].mReqPhase |= phase;
  }

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
    ASSERT_COND( inv );
    // NOT ゲートを表す LUT を作る．
    ymuint src_id = node_info.mMapNode[0];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    mapnetwork.connect(src_id, node_id, 0);
    node_info.mMapNode[1] = node_id;
    return node_id;
  }

  // 反対側の極性のノードができていたら NOT ゲートを作る．
  if ( node_info.mMapNode[idx ^ 1] ) {
    // NOT ゲートを表す LUT を作る．
    ymuint src_id = node_info.mMapNode[idx ^ 1];
    TvFunc tv = TvFunc::nega_literal(1, VarId(0));
    node_id = mapnetwork.add_tv(string(), tv);
    cout << "extra 2" << endl;
    mapnetwork.connect(src_id, node_id, 0);
    node_info.mMapNode[idx] = node_id;
    return node_id;
  }

  // node を根とするカットを取り出す．
  const vector<const SbjNode*>& inputs = node_info.mInputs;

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = inputs.size();
  vector<bool> input_inv(ni, false);
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = inputs[i];
    bool inv = false;
    if ( mNodeInfo[inode->id()].mReqPhase == 2 ) {
      inv = true;
    }
    input_inv[i] = inv;
    back_trace(inode, inv, mapnetwork);
  }

  // カットの実現している関数の真理値表を得る．
  TvFunc tv = make_tv(node, inputs, inv, input_inv);

  // 新しいノードを作り mNodeMap に登録する．
  node_id = mapnetwork.add_tv(string(), tv);
  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = inputs[i];
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

END_NAMESPACE_YM_LUTMAP2
