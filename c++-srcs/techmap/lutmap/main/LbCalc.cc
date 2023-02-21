
/// @file LbCalc.cc
/// @brief LbCalc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
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

// @breif 下界の計算をする．
SizeType
LbCalc::lower_bound(
  const SbjGraph& sbjgraph,
  const CutHolder& cut_holder
)
{
  SizeType node_num = sbjgraph.node_num();

  // 作業領域を初期化する．
  mMark.clear();
  mMark.resize(node_num, false);

  DgGraph dg{node_num};

  vector<SizeType> max_value(node_num, 0);
  for ( SizeType i = 0; i < node_num; ++ i ) {
    auto node = sbjgraph.node(i);
    if ( !node->is_logic() ) {
      auto dgnode = dg.node(node->id());
      dgnode->inactivate();
    }

    for ( auto cut: cut_holder.cut_list(node) ) {
      // カットがカバーしているノードを求める．
      auto node_list = get_node_list(cut);
      SizeType n = node_list.size();
      // ノード数をこのカットの価値とする．
      for ( auto node1: node_list ) {
	// カバーしているカットの価値の最大値を求める．
	if ( max_value[node1->id()] < n ) {
	  max_value[node1->id()] = n;
	}
      }

      // このカットでカバーされているノード対を隣接リストに入れる．
      for ( SizeType j1 = 0; j1 < n - 1; ++ j1 ) {
	SizeType id1 = node_list[j1]->id();
	for ( SizeType j2 = j1 + 1; j2 < n; ++ j2 ) {
	  SizeType id2 = node_list[j2]->id();
	  dg.connect(id1, id2);
	}
      }
    }
  }

  // 各ノードの最大値の逆数が下界となる．
  double d_lb1 = 0.0;
  for ( SizeType i = 0; i < node_num; ++ i ) {
    SizeType n = max_value[i];
    if ( n == 0 ) {
      continue;
    }
    double l = 1.0 / n;
    d_lb1 += l;
  }
  SizeType lb1 = static_cast<SizeType>(ceil(d_lb1));

  SizeType lb2 = dg.get_mis_size();

  auto lb = std::max(lb1, lb2);

  return lb;
}

// @brief カットのカバーしているノードを求める．
vector<const SbjNode*>
LbCalc::get_node_list(
  const Cut* cut
)
{
  vector<const SbjNode*> node_list;
  auto root = cut->root();
  node_list.push_back(root);
  mMark[root->id()] = true;

  // inputs[] のノードに印をつけておく．
  SizeType ni = cut->input_num();
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node = cut->input(i);
    mMark[node->id()] = true;
  }
  for ( SizeType rpos = 0; rpos < node_list.size(); ++ rpos ) {
    auto node = node_list[rpos];
    ASSERT_COND( node->is_logic() );

    auto inode0 = node->fanin(0);
    if ( !mMark[inode0->id()] ) {
      mMark[inode0->id()] = true;
      node_list.push_back(inode0);
    }

    auto inode1 = node->fanin(1);
    if ( !mMark[inode1->id()] ) {
      mMark[inode1->id()] = true;
      node_list.push_back(inode1);
    }
  }

  // マークを消しておく．
  for ( auto node: node_list ) {
    mMark[node->id()] = false;
  }
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node = cut->input(i);
    mMark[node->id()] = false;
  }

  return node_list;
}

END_NAMESPACE_LUTMAP
