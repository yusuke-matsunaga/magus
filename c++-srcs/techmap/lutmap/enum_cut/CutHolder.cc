
/// @file lutmap/CutHolder.cc
/// @brief CutHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CutHolder.h"
#include "Cut.h"
#include "CutList.h"


BEGIN_NAMESPACE_LUTMAP

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
CutHolder::all_init(
  const SbjGraph& sbjgraph,
  SizeType limit
)
{
  clear();
  SizeType n = sbjgraph.node_num();
  mLimit = limit;
  mCutList = new CutList[n];
}

void
CutHolder::node_init(
  const SbjNode* node,
  SizeType pos
)
{
}

void
CutHolder::found(
  const SbjNode* root
)
{
}

void
CutHolder::found(
  const SbjNode* root,
  SizeType ni,
  const SbjNode* inputs[]
)
{
  auto cut = mMgr.new_cut(root, ni, inputs);
  mCutList[root->id()].push_back(cut);
}

void
CutHolder::node_end(
  const SbjNode* node,
  SizeType pos,
  SizeType ncuts
)
{
}

// 最後に呼ばれる関数
void
CutHolder::all_end(
  const SbjGraph& sbjgraph,
  SizeType limit
)
{
}

END_NAMESPACE_LUTMAP
