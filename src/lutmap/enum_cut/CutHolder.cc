
/// @file lutmap/CutHolder.cc
/// @brief CutHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "CutHolder.h"
#include "Cut.h"
#include "CutList.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
CutHolder::CutHolder()
{
  mCutList = nullptr;
}

// デストラクタ
CutHolder::~CutHolder()
{
  clear();
}

// @brief 保持しているカットのリストを削除する．
void
CutHolder::clear()
{
  delete [] mCutList;
  mMgr.clear();
}

// 最初に呼ばれる関数
void
CutHolder::all_init(const SbjGraph& sbjgraph,
		    int limit)
{
  clear();
  int n = sbjgraph.node_num();
  mLimit = limit;
  mCutList = new CutList[n];
}

void
CutHolder::node_init(const SbjNode* node,
		     int pos)
{
}

void
CutHolder::found(const SbjNode* root)
{
}

void
CutHolder::found(const SbjNode* root,
		 int ni,
		 const SbjNode* inputs[])
{
  Cut* cut = mMgr.new_cut(root, ni, inputs);
  mCutList[root->id()].push_back(cut);
}

void
CutHolder::node_end(const SbjNode* node,
		    int pos,
		    int ncuts)
{
}

// 最後に呼ばれる関数
void
CutHolder::all_end(const SbjGraph& sbjgraph,
		   int limit)
{
}

END_NAMESPACE_YM_LUTMAP
