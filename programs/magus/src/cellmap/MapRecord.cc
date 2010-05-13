
/// @file magus/cellmap/MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "MapRecord.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CellNetwork.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

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
  size_t n = src.mNodeInfo.size();
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (size_t i = 0; i < n; ++ i) {
    mNodeInfo[i].mCut = src.mNodeInfo[i].mCut;
  }
}

// @blif カットを記録する．
// @param[in] node 該当のノード
// @param[in] cut 対応するカット
// @param[in] io_map 入出力のマッピング
void
MapRecord::set_cut(SbjNode* node,
		   const Cut* cut,
		   const NpnMap& io_map)
{
  mNodeInfo[node->id()].mCut = cut;
}

// @brief カットを取り出す．
// @param[in] node 該当のノード
const Cut*
MapRecord::get_cut(SbjNode* node)
{
  return mNodeInfo[node->id()].mCut;
}

// @brief マッピング結果を CnGraph にセットする．
void
MapRecord::gen_mapgraph(const SbjGraph& sbjgraph,
			CnGraph& mapgraph,
			int& cell_num)
{
  mapgraph.clear();
  
  // 外部入力の生成
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    CnNode* mapnode = mapgraph.new_input(node->name());
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode[0] = mapnode;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    bool inv = onode->output_inv();
    const string& name = onode->name();
    CnNode* mapnode = NULL;
    if ( node ) {
      mapnode = back_trace(node, inv, mapgraph, name);
    }
    else {
      LogExpr expr;
      if ( inv ) {
	expr = LogExpr::make_one();
      }
      else {
	expr = LogExpr::make_zero();
      }
      mapnode = mapgraph.new_cell(name, vector<CnNode*>(0), expr);
    }
    mapgraph.new_output(name, mapnode);
  }
  cell_num = mapgraph.n_cnodes();
}

BEGIN_NONAMESPACE

// 論理式から真理値表を作る．
void
make_tv(size_t ni,
	const LogExpr& expr,
	vector<int>& tv)
{
  size_t np = 1 << ni;
  
  vector<ymulong> vals(ni);
  for (size_t i = 0; i < ni; ++ i) {
    vals[i] = 0UL;
  }

  tv.resize(np);
  ymulong s = 1UL;
  size_t p0 = 0;
  for (size_t p = 0; p < np; ++ p) {
    for (size_t i = 0; i < ni; ++ i) {
      if ( p & (1U << i) ) {
	vals[i] |= s;
      }
    }
    s <<= 1;
    if ( s == 0UL ) {
      ymulong tmp = expr.eval(vals);
      for (size_t p1 = p0; p1 <= p; ++ p1) {
	if ( tmp & (1UL << (p1 - p0)) ) {
	  tv[p1] = 1;
	}
	else {
	  tv[p1] = 0;
	}
      }
      s = 1UL;
      p0 = p + 1;
      for (size_t i = 0; i < ni; ++ i) {
	vals[i] = 0UL;
      }
    }
  }
  if ( s != 1UL ) {
    ymulong tmp = expr.eval(vals);
    for (size_t p1 = p0; p1 < np; ++ p1) {
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

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
CnNode*
MapRecord::back_trace(SbjNode* node,
		      bool inv,
		      CnGraph& mapnetwork,
		      const string& oname)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  size_t idx = (inv) ? 1 : 0;
  CnNode* mapnode = node_info.mMapNode[idx];
  if ( mapnode ) {
    // すでに生成済みならそのノードを返す．
    return mapnode;
  }

  if ( node->is_input() ) {
    // ということは inv = true のはず．
    assert_cond(inv, __FILE__, __LINE__);
    // NOT ゲートを表す LUT を作る．
    vector<CnNode*> inputs(1, node_info.mMapNode[0]);
    LogExpr expr = LogExpr::make_negaliteral(0);
    CnNode* mapnode1 = mapnetwork.new_cell(oname, inputs, expr);
    node_info.mMapNode[1] = mapnode1;
    return mapnode1;
  }
  
  // node を根とするカットを取り出す．
  const Cut* cut = node_info.mCut;
  
  // その入力に対応するノードを再帰的に生成する．
  size_t ni = cut->ni();
  for (size_t i = 0; i < ni; ++ i) {
    SbjNode* inode = cut->input(i);
    back_trace(inode, false, mapnetwork, string());
  }

  mTmpFanins.clear();
  mTmpFanins.resize(ni);
  for (size_t i = 0; i < ni; ++ i) {
    SbjNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    CnNode* imapnode = inode_info.mMapNode[0];
    mTmpFanins[i] = imapnode;
  }

  LogExpr expr = cut->expr();
  if ( inv ) {
    expr = ~expr;
  }
  
  // 新しいノードを作り mNodeMap に登録する．
  string name = (inv) ? oname : node->name();
  mapnode = mapnetwork.new_cell(name, mTmpFanins, expr);
  node_info.mMapNode[idx] = mapnode;
  
  return mapnode;
}

END_NAMESPACE_MAGUS_CELLMAP
