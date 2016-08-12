
/// @file MapEst.cc
/// @brief MapEst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MapEst.h"
#include "MapRecord.h"
#include "Cut.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
MapEst::MapEst()
{
}

// デストラクタ
MapEst::~MapEst()
{
}

// @brief マッピング結果から見積もりを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] record マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 最大段数
void
MapEst::estimate(const SbjGraph& sbjgraph,
		 const MapRecord& record,
		 ymuint& lut_num,
		 ymuint& depth)
{
  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id());

  // 外部入力の生成
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapCount[0] = 1;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  lut_num = 0;
  ymuint no = sbjgraph.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    if ( node && node->is_logic() ) {
      if ( record.get_cut(node) == nullptr ) {
	lut_num = -1;
	break;
      }
      bool inv = onode->output_fanin_inv();
      lut_num += back_trace(node, inv, record);
    }
  }
}

// @brief 最終結果を作るためのバックトレースを行う．
// @param[in] node 対象のノード
// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
// @param[in] record マッピング結果
ymuint
MapEst::back_trace(const SbjNode* node,
		   bool inv,
		   const MapRecord& record)
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
  const Cut* cut = record.get_cut(node);

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
    lut_num += back_trace(inode, false, record);
  }

  return lut_num;
}

// @brief cut でカバーされるノードの mCovCount を一つ増やす．
// @param[in] node 対象のノード
// @param[in] cut カット
void
MapEst::mark_cover(const SbjNode* node,
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

// @brief mark_cover でつけた mTmpFlag を下ろす．
// @param[in] node 対象のノード
void
MapEst::clear_mark(const SbjNode* node)
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

END_NAMESPACE_YM_LUTMAP
