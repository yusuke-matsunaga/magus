
/// @file LbCalc.cc
/// @brief LbCalc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "LbCalc.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "CutList.h"
#include "DgGraph.h"
#include "DgNode.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス LbCalc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LbCalc::LbCalc()
{
}

// @brief デストラクタ
LbCalc::~LbCalc()
{
}

// @breif 下界の計算をする．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットを保持するオブジェクト
int
LbCalc::lower_bound(const SbjGraph& sbjgraph,
		    const CutHolder& cut_holder)
{
  int node_num = sbjgraph.node_num();

  // 作業領域を初期化する．
  mMark.clear();
  mMark.resize(node_num, false);

  DgGraph dg(node_num);

  vector<int> max_value(node_num, 0);
  for ( int i = 0; i < node_num; ++ i ) {
    const SbjNode* node = sbjgraph.node(i);
    if ( !node->is_logic() ) {
      DgNode* dgnode = dg.node(node->id());
      dgnode->inactivate();
    }

    for ( auto cut: cut_holder.cut_list(node) ) {
      // カットがカバーしているノードを求める．
      vector<const SbjNode*> node_list;
      get_node_list(cut, node_list);
      int n = node_list.size();
      // ノード数をこのカットの価値とする．
      for ( auto node1: node_list ) {
	// カバーしているカットの価値の最大値を求める．
	if ( max_value[node1->id()] < n ) {
	  max_value[node1->id()] = n;
	}
      }

      // このカットでカバーされているノード対を隣接リストに入れる．
      for ( int j1 = 0; j1 < n - 1; ++ j1 ) {
	int id1 = node_list[j1]->id();
	for ( int j2 = j1 + 1; j2 < n; ++ j2 ) {
	  int id2 = node_list[j2]->id();
	  dg.connect(id1, id2);
	}
      }
    }
  }

  // 各ノードの最大値の逆数が下界となる．
  double d_lb1 = 0.0;
  for ( int i = 0; i < node_num; ++ i ) {
    int n = max_value[i];
    if ( n == 0 ) {
      continue;
    }
    double l = 1.0 / n;
    d_lb1 += l;
  }
  int lb1 = static_cast<int>(ceil(d_lb1));

  int lb2 = dg.get_mis_size();

  int lb = lb1 > lb2 ? lb1 : lb2;

  return lb;
}

// @brief カットのカバーしているノードを求める．
void
LbCalc::get_node_list(const Cut* cut,
		      vector<const SbjNode*>& node_list)
{
  node_list.clear();
  const SbjNode* root = cut->root();
  node_list.push_back(root);
  mMark[root->id()] = true;

  // inputs[] のノードに印をつけておく．
  int ni = cut->input_num();
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = cut->input(i);
    mMark[node->id()] = true;
  }
  for ( int rpos = 0; rpos < node_list.size(); ++ rpos ) {
    const SbjNode* node = node_list[rpos];
    ASSERT_COND( node->is_logic() );

    const SbjNode* inode0 = node->fanin(0);
    if ( !mMark[inode0->id()] ) {
      mMark[inode0->id()] = true;
      node_list.push_back(inode0);
    }

    const SbjNode* inode1 = node->fanin(1);
    if ( !mMark[inode1->id()] ) {
      mMark[inode1->id()] = true;
      node_list.push_back(inode1);
    }
  }

  // マークを消しておく．
  for ( auto node: node_list ) {
    mMark[node->id()] = false;
  }
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = cut->input(i);
    mMark[node->id()] = false;
  }
}

END_NAMESPACE_LUTMAP
