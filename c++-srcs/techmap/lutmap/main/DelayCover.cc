
/// @file DelayCover.cc
/// @brief DelayCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DelayCover.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_LUTMAP

// コンストラクタ
DelayCover::DelayCover(
  bool fanout_mode,
  int slack
) : DagCover{fanout_mode},
    mSlack{slack}
{
}

// デストラクタ
DelayCover::~DelayCover()
{
}

// @brief best cut の記録を行う．
void
DelayCover::record_cuts(
  const SbjGraph& sbjgraph,
  const CutHolder& cut_holder,
  MapRecord& maprec
)
{
  SizeType n = sbjgraph.node_num();

  maprec.init(sbjgraph);

  // 作業領域の初期化
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for ( int i = 0; i < n; ++ i ) {
    mNodeInfo[i].mCostList.set_mgr(&mCostMgr);
  }
  SizeType limit = cut_holder.limit();
  mWeight.resize(limit);
  mIcostLists.resize(limit);
  mIcostListEnds.resize(limit);

  // 入力のコストの設定
  SizeType ni = sbjgraph.input_num();
  for ( int i = 0; i < ni; ++ i ) {
    auto node = sbjgraph.input(i);
    auto& t = mNodeInfo[node->id()];
    t.mCostList.insert(nullptr, 0, 0.0);
    t.mMinDepth = 0;
  }

  // 各ノードごとにカットを記録
  int nl = sbjgraph.logic_num();
  for ( int i = 0; i < nl; ++ i ) {
    const SbjNode* node = sbjgraph.logic(i);
    record(node, cut_holder);
  }

  // 最小段数の最大値をもとめる．
  SizeType no = sbjgraph.output_num();
  vector<const SbjNode*> onode_list;
  onode_list.reserve(no);
  SizeType min_depth = 0;
  for ( int i = 0; i < no; ++ i ) {
    auto onode = sbjgraph.output(i);
    auto node = onode->output_fanin();
    if ( node == nullptr) continue;
    if ( node->is_logic() ) {
      onode_list.push_back(node);
    }
    auto& node_info = mNodeInfo[node->id()];
    SizeType depth = node_info.mMinDepth;
    if ( min_depth < depth ) {
      min_depth = depth;
    }
  }

  // それに slack を足したものが制約となる．
  min_depth += mSlack;
  for ( auto node: onode_list ) {
    mNodeInfo[node->id()].mReqDepth = min_depth;
  }

  // 要求された段数制約を満たす中でコスト最小の解を選ぶ．
  for ( int i = 0; i < nl; ++ i ) {
    auto node = sbjgraph.logic(nl - i - 1);
    select(node, maprec);
  }
}

// node のカットを選択する．
void
DelayCover::record(
  const SbjNode* node,
  const CutHolder& cut_holder
)
{
  SizeType min_depth = INT_MAX;
  auto& t = mNodeInfo[node->id()];
  for ( auto cut: cut_holder.cut_list(node) ) {
    SizeType ni = cut->input_num();

    if ( fanout_mode() ) {
      // ファンアウトモード
      for ( int i = 0; i < ni; ++ i ) {
	auto inode = cut->input(i);
	mWeight[i] = 1.0 / inode->fanout_num();
      }
    }
    else {
      // フローモード
      for ( int i = 0; i < ni; ++ i ) {
	mWeight[i] = 0.0;
      }
      calc_weight(node, cut, 1.0);
    }

    SizeType max_input_depth = 0;
    for ( int i = 0; i < ni; ++ i ) {
      auto inode = cut->input(i);
      auto& u = mNodeInfo[inode->id()];
      if ( max_input_depth < u.mMinDepth ) {
	max_input_depth = u.mMinDepth;
      }
      mIcostLists[i] = u.mCostList.begin();
      mIcostListEnds[i] = u.mCostList.end();
    }

    SizeType cur_depth = max_input_depth + 1;
    if ( min_depth > cur_depth ) {
      min_depth = cur_depth;
    }

    // mIcostLists から解を作る．
    for ( ; ; ) {
      // 各入力のなかでもっとも深い値を求める．
      SizeType idepth = 0;
      double area = 1.0;
      bool empty = false;
      for ( int i = 0; i < ni; ++ i ) {
	if ( mIcostLists[i] == mIcostListEnds[i] ) {
	  empty = true;
	  break;
	}
	auto cost = *mIcostLists[i];
	if ( idepth < cost->depth() ) {
	  idepth = cost->depth();
	}
	area += cost->area() * mWeight[i];
      }
      if ( empty ) {
	break;
      }
      SizeType depth = idepth + 1;

      // (depth, area) を登録
      t.mCostList.insert(cut, depth, area);

      // 深さが idepth に等しい解を次に進める．
      for ( int i = 0; i < ni; ++ i ) {
	auto cost = *mIcostLists[i];
	if ( cost->depth() == idepth ) {
	  ++ mIcostLists[i];
	}
      }
    }
  }
  t.mMinDepth = min_depth;
}

// node から各入力にいたる経路の重みを計算する．
void
DelayCover::calc_weight(
  const SbjNode* node,
  const Cut* cut,
  double cur_weight
)
{
  for ( ; ; ) {
    for ( int i = 0; i < cut->input_num(); ++ i ) {
      if ( cut->input(i) == node ) {
	if  ( !node->pomark() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    auto inode0 = node->fanin(0);
    calc_weight(inode0, cut, cur_weight / inode0->fanout_num());
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

// node のカットを選択する．
void
DelayCover::select(
  const SbjNode* node,
  MapRecord& maprec
)
{
  auto& t = mNodeInfo[node->id()];
  SizeType rd = t.mReqDepth;
  if ( rd == 0 ) {
    // このノードは必要ではない．
    return;
  }

  const Cut* cut = nullptr;
  for ( auto cost: t.mCostList ) {
    if ( cost->depth() <= rd ) {
      cut = cost->cut();
      break;
    }
  }
  ASSERT_COND( cut );
  maprec.set_cut(node, cut);
  -- rd;
  for ( int i = 0; i < cut->input_num(); ++ i ) {
    auto inode = cut->input(i);
    auto& u = mNodeInfo[inode->id()];
    if ( u.mReqDepth == 0 || u.mReqDepth > rd ) {
      u.mReqDepth = rd;
    }
  }
}

END_NAMESPACE_LUTMAP
