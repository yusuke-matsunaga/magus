
/// @file libym_techmap/lutmap/MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNode.h"
#include "LnGraph.h"
#include "Cut.h"


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
MapRecord::init(const BdnMgr& sbjgraph)
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
MapRecord::set_cut(const BdnNode* node,
		   const Cut* cut)
{
  mNodeInfo[node->id()].mCut = cut;
}

// @brief カットを取り出す．
// @param[in] node 該当のノード
const Cut*
MapRecord::get_cut(const BdnNode* node)
{
  return mNodeInfo[node->id()].mCut;
}

// @brief マッピング結果を LnGraph にセットする．
void
MapRecord::gen_mapgraph(const BdnMgr& sbjgraph,
			LnGraph& mapgraph,
			ymuint& lut_num,
			ymuint& depth)
{
  mapgraph.clear();

  mapgraph.set_name(sbjgraph.name());

  // 外部入力の生成
  const BdnNodeList& input_list = sbjgraph.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    LnNode* mapnode = mapgraph.new_input();
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = mapnode;
    node_info.mDepth = 0;
  }

#if 0
  // DFFの生成
  const BdnNodeList& dff_list = sbjgraph.dff_list();
  for (BdnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnNode* node = *p;
    LnNode* mapnode = mapgraph.new_dff();
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = mapnode;
    node_info.mDepth = 0;
  }
#endif

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  const BdnNodeList& output_list = sbjgraph.output_list();
  int max_depth = 0;
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* onode = *p;
    const BdnNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    LnNode* mapnode = nullptr;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph);
      int depth1 = mNodeInfo[node->id()].mDepth;
      if ( max_depth < depth1 ) {
	max_depth = depth1;
      }
    }
    else {
      vector<int> tv(1);
      if ( inv ) {
	tv[0] = 1;
      }
      else {
	tv[0] = 0;
      }
      mapnode = mapgraph.new_lut(vector<LnNode*>(0), tv);
    }
    LnNode* omapnode = mapgraph.new_output(mapnode);
    mNodeInfo[onode->id()].mMapNode[0] = omapnode;
  }

#if 0
  // DFFからバックトレースを行い全ノードの生成を行う．
  for (BdnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnNode* onode = *p;

    {
      const BdnNode* node = onode->fanin_data();
      bool inv = onode->fanin_data_inv();
      LnNode* mapnode = nullptr;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	int depth1 = mNodeInfo[node->id()].mDepth;
	if ( max_depth < depth1 ) {
	  max_depth = depth1;
	}
      }
      else {
	vector<int> tv(1);
	if ( inv ) {
	  tv[0] = 1;
	}
	else {
	  tv[0] = 0;
	}
	mapnode = mapgraph.new_lut(vector<LnNode*>(0), tv);
      }
      LnNode* omapnode = mNodeInfo[onode->id()].mMapNode[0];
      mapgraph.set_dff_input(omapnode, mapnode);
    }
    {
      const BdnNode* node = onode->fanin_clock();
      bool inv = onode->fanin_clock_inv();
      LnNode* mapnode = nullptr;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	int depth1 = mNodeInfo[node->id()].mDepth;
	if ( max_depth < depth1 ) {
	  max_depth = depth1;
	}
	LnNode* omapnode = mNodeInfo[onode->id()].mMapNode[0];
	mapgraph.set_dff_clock(omapnode, mapnode);
      }
    }
    {
      const BdnNode* node = onode->fanin_set();
      bool inv = onode->fanin_set_inv();
      LnNode* mapnode = nullptr;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	int depth1 = mNodeInfo[node->id()].mDepth;
	if ( max_depth < depth1 ) {
	  max_depth = depth1;
	}
	LnNode* omapnode = mNodeInfo[onode->id()].mMapNode[0];
	mapgraph.set_dff_set(omapnode, mapnode);
      }
    }
    {
      const BdnNode* node = onode->fanin_rst();
      bool inv = onode->fanin_rst_inv();
      LnNode* mapnode = nullptr;
      if ( node ) {
	mapnode = back_trace(node, inv, mapgraph);
	int depth1 = mNodeInfo[node->id()].mDepth;
	if ( max_depth < depth1 ) {
	  max_depth = depth1;
	}
	LnNode* omapnode = mNodeInfo[onode->id()].mMapNode[0];
	mapgraph.set_dff_rst(omapnode, mapnode);
      }
    }
  }
#endif

  // ポートの生成
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BdnPort* sbjport = sbjgraph.port(i);
    ymuint nb = sbjport->bit_width();
    vector<LnNode*> tmp(nb);
    for (ymuint j = 0; j < nb; ++ j) {
      LnNode* node = nullptr;
      const BdnNode* input = sbjport->input(j);
      const BdnNode* output = sbjport->output(j);
      if ( input ) {
	node = mNodeInfo[input->id()].mMapNode[0];
	ASSERT_COND( node != nullptr );
      }
      else if ( output ) {
	node = mNodeInfo[output->id()].mMapNode[0];
	ASSERT_COND( node != nullptr );
      }
      tmp[j] = node;
    }
    mapgraph.add_port(sbjport->name(), tmp);
  }

  lut_num = mapgraph.lnode_num();
  depth = max_depth;
}

#if 0
BEGIN_NONAMESPACE

// 論理式から真理値表を作る．
void
make_tv(ymuint ni,
	const Expr& expr,
	vector<int>& tv)
{
  ymuint np = 1 << ni;

  vector<ymulong> vals(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vals[i] = 0UL;
  }

  tv.resize(np);
  ymulong s = 1UL;
  ymuint p0 = 0;
  for (ymuint p = 0; p < np; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( p & (1U << i) ) {
	vals[i] |= s;
      }
    }
    s <<= 1;
    if ( s == 0UL ) {
      ymulong tmp = expr.eval(vals);
      for (ymuint p1 = p0; p1 <= p; ++ p1) {
	if ( tmp & (1UL << (p1 - p0)) ) {
	  tv[p1] = 1;
	}
	else {
	  tv[p1] = 0;
	}
      }
      s = 1UL;
      p0 = p + 1;
      for (ymuint i = 0; i < ni; ++ i) {
	vals[i] = 0UL;
      }
    }
  }
  if ( s != 1UL ) {
    ymulong tmp = expr.eval(vals);
    for (ymuint p1 = p0; p1 < np; ++ p1) {
      if ( tmp & (1UL << (p1 - p0)) ) {
	tv[p1] = 1;
      }
      else {
	tv[p1] = 0;
      }
    }
  }
}

END_NONAMESPACE
#endif

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
LnNode*
MapRecord::back_trace(const BdnNode* node,
		      bool inv,
		      LnGraph& mapnetwork)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint idx = (inv) ? 1 : 0;
  LnNode* mapnode = node_info.mMapNode[idx];
  if ( mapnode ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    ASSERT_COND(inv );
    // NOT ゲートを表す LUT を作る．
    vector<LnNode*> inputs(1, node_info.mMapNode[0]);
    vector<int> tv(2);
    tv[0] = 1;
    tv[1] = 0;
    LnNode* mapnode1 = mapnetwork.new_lut(inputs, tv);
    node_info.mMapNode[1] = mapnode1;
    return mapnode1;
  }

  // node を根とするカットを取り出す．
  const Cut* cut = node_info.mCut;

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = cut->input(i);
    back_trace(inode, false, mapnetwork);
  }

  mTmpFanins.clear();
  mTmpFanins.resize(ni);
  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    LnNode* imapnode = inode_info.mMapNode[0];
    mTmpFanins[i] = imapnode;
    int idepth1 = inode_info.mDepth;
    if ( idepth < idepth1 ) {
      idepth = idepth1;
    }
  }

  ymuint np = 1 << ni;
  vector<int> tv(np);
#if 0
  Expr expr = cut->expr();
  if ( inv ) {
    expr = ~expr;
  }

  // 論理式から真理値表を作る．
  make_tv(ni, expr, tv);
#else
  // カットの実現している関数の真理値表を得る．
  cut->make_tv(inv, tv);

#if 0
  { // make_tv() の検証用のコード
    Expr expr = cut->expr();
    if ( inv ) {
      expr = ~expr;
    }

    // 論理式から真理値表を作る．
    vector<int> tv2(np);
    make_tv(ni, expr, tv2);
    for (ymuint i = 0; i < np; ++ i) {
      ASSERT_COND( tv[i] == tv2[i] );
    }
  }
#endif
#endif

  // 新しいノードを作り mNodeMap に登録する．
  mapnode = mapnetwork.new_lut(mTmpFanins, tv);
  node_info.mMapNode[idx] = mapnode;
  node_info.mDepth = idepth + 1;

  return mapnode;
}

// @brief マッピング結果の LUT 数を見積もる．
int
MapRecord::estimate(const BdnMgr& sbjgraph)
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
  const BdnNodeList& input_list = sbjgraph.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapCount[0] = 1;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  int lut_num = 0;
  const BdnNodeList& output_list = sbjgraph.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* onode = *p;
    const BdnNode* node = onode->output_fanin();
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

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
int
MapRecord::back_trace2(const BdnNode* node,
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
    const BdnNode* leaf = cut->input(i);
    mNodeInfo[leaf->id()].mTmpFlag = true;
  }
  mark_cover(node, cut);
  clear_mark(node);

  int lut_num = 1;
  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = cut->input(i);
    lut_num += back_trace2(inode, false);
  }

  return lut_num;
}

// @brief 直前の estimate の結果 node が fanout node なら true を返す．
bool
MapRecord::check_fonode(const BdnNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  return (node_info.mMapCount[0] + node_info.mMapCount[1]) > 1;
}

// @brief 直前の estimate の結果で node のカバーされている回数を返す．
int
MapRecord::cover_count(const BdnNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  return node_info.mCovCount;
}

// cut でカバーされるノードの mCovCount を一つ増やす．
void
MapRecord::mark_cover(const BdnNode* node,
		      const Cut* cut)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( !node_info.mTmpFlag ) {
    node_info.mTmpFlag = true;
    ++ node_info.mCovCount;
    mark_cover(node->fanin0(), cut);
    mark_cover(node->fanin1(), cut);
  }
}

// mark_cover でつけた mTmpFlag を下ろす．
void
MapRecord::clear_mark(const BdnNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mTmpFlag ) {
    node_info.mTmpFlag = false;
    if ( node->is_logic() ) {
      clear_mark(node->fanin0());
      clear_mark(node->fanin1());
    }
  }
}

END_NAMESPACE_YM_LUTMAP
