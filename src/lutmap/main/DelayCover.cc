
/// @file DelayCover.cc
/// @brief DelayCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "DelayCover.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
DelayCover::DelayCover(int mode)
{
  mMode = mode;
}

// デストラクタ
DelayCover::~DelayCover()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[in] slack 最小段数に対するスラック
// @param[out] maprec マッピング結果を記録するオブジェクト
void
DelayCover::record_cuts(const SbjGraph& sbjgraph,
			const CutHolder& cut_holder,
			int slack,
			MapRecord& maprec)
{
  int n = sbjgraph.node_num();

  maprec.init(sbjgraph);

  // 作業領域の初期化
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for ( int i = 0; i < n; ++ i ) {
    mNodeInfo[i].mCostList.set_mgr(&mCostMgr);
  }
  int limit = cut_holder.limit();
  mWeight.resize(limit);
  mIcostLists.resize(limit);

  // 入力のコストの設定
  int ni = sbjgraph.input_num();
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& t = mNodeInfo[node->id()];
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
  int no = sbjgraph.output_num();
  vector<const SbjNode*> onode_list;
  onode_list.reserve(no);
  int min_depth = 0;
  for ( int i = 0; i < no; ++ i ) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    if ( node == nullptr) continue;
    if ( node->is_logic() ) {
      onode_list.push_back(node);
    }
    NodeInfo& node_info = mNodeInfo[node->id()];
    int depth = node_info.mMinDepth;
    if ( min_depth < depth ) {
      min_depth = depth;
    }
  }

  // それに slack を足したものが制約となる．
  min_depth += slack;
  for ( int i = 0; i < onode_list.size(); ++ i ) {
    const SbjNode* node = onode_list[i];
    mNodeInfo[node->id()].mReqDepth = min_depth;
  }

  // 要求された段数制約を満たす中でコスト最小の解を選ぶ．
  for ( int i = 0; i < nl; ++ i ) {
    const SbjNode* node = sbjgraph.logic(nl - i - 1);
    select(node, maprec);
  }
}

// node のカットを選択する．
void
DelayCover::record(const SbjNode* node,
		   const CutHolder& cut_holder)
{
  int min_depth = INT_MAX;
  NodeInfo& t = mNodeInfo[node->id()];
  for ( auto cut: cut_holder.cut_list(node) ) {
    int ni = cut->input_num();

    if ( mMode & 1 ) {
      // ファンアウトモード
      for ( int i = 0; i < ni; ++ i ) {
	const SbjNode* inode = cut->input(i);
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

    int max_input_depth = 0;
    for ( int i = 0; i < ni; ++ i ) {
      const SbjNode* inode = cut->input(i);
      NodeInfo& u = mNodeInfo[inode->id()];
      if ( max_input_depth < u.mMinDepth ) {
	max_input_depth = u.mMinDepth;
      }
      mIcostLists[i] = u.mCostList.begin();
    }

    int cur_depth = max_input_depth + 1;
    if ( min_depth > cur_depth ) {
      min_depth = cur_depth;
    }

    // mIcostLists から解を作る．
    for ( ; ; ) {
      // 各入力のなかでもっとも深い値を求める．
      int idepth = 0;
      double area = 1.0;
      bool empty = false;
      for ( int i = 0; i < ni; ++ i ) {
	if ( mIcostLists[i].is_end() ) {
	  empty = true;
	  break;
	}
	ADCost<double>* cost = *mIcostLists[i];
	if ( idepth < cost->depth() ) {
	  idepth = cost->depth();
	}
	area += cost->area() * mWeight[i];
      }
      if ( empty ) {
	break;
      }
      int depth = idepth + 1;

      // (depth, area) を登録
      t.mCostList.insert(cut, depth, area);

      // 深さが idepth に等しい解を次に進める．
      for ( int i = 0; i < ni; ++ i ) {
	ADCost<double>* cost = *mIcostLists[i];
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
DelayCover::calc_weight(const SbjNode* node,
			const Cut* cut,
			double cur_weight)
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
    const SbjNode* inode0 = node->fanin(0);
    calc_weight(inode0, cut, cur_weight / inode0->fanout_num());
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

// node のカットを選択する．
void
DelayCover::select(const SbjNode* node,
		   MapRecord& maprec)
{
  NodeInfo& t = mNodeInfo[node->id()];
  int rd = t.mReqDepth;
  if ( rd == 0 ) {
    // このノードは必要ではない．
    return;
  }

  const Cut* cut = nullptr;
  for ( ADCostIterator<double> p = t.mCostList.begin();
       !p.is_end(); ++ p) {
    ADCost<double>* cost = *p;
    if ( cost->depth() <= rd ) {
      cut = cost->cut();
      break;
    }
  }
  ASSERT_COND( cut );
  maprec.set_cut(node, cut);
  -- rd;
  for ( int i = 0; i < cut->input_num(); ++ i ) {
    const SbjNode* inode = cut->input(i);
    NodeInfo& u = mNodeInfo[inode->id()];
    if ( u.mReqDepth == 0 || u.mReqDepth > rd ) {
      u.mReqDepth = rd;
    }
  }
}

END_NAMESPACE_YM_LUTMAP
