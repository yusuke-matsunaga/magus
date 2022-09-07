
/// @file lutmap/EnumCutOp.cc
/// @brief EnumCutOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "EnumCutOp.h"
#include "EnumCut.h"

//#define DEBUG_ENUM_RECUR


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// クラス EnumCutOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EnumCutOp::EnumCutOp()
{
}

// @brief デストラクタ
EnumCutOp::~EnumCutOp()
{
}

// @brief カットの列挙を行う．
SizeType
EnumCutOp::enum_cut(
  const SbjGraph& sbjgraph,
  SizeType limit
)
{
  EnumCut ec;
  return ec(sbjgraph, limit, this);
}

// @brief 処理の最初に呼ばれる関数
void
EnumCutOp::all_init(
  const SbjGraph& sbjgraph,
  SizeType limit
)
{
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
void
EnumCutOp::node_init(
  const SbjNode* node,
  SizeType cur_pos
)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
void
EnumCutOp::found(
  const SbjNode* root
)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
void
EnumCutOp::found(
  const SbjNode* root,
  SizeType ni,
  const SbjNode* inputs[]
)
{
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
void
EnumCutOp::node_end(
  const SbjNode* node,
  SizeType cur_pos,
  SizeType ncuts
)
{
}

// @brief 処理の最後に呼ばれる関数
void
EnumCutOp::all_end(
  const SbjGraph& sbjgraph,
  SizeType limit
)
{
}

END_NAMESPACE_LUTMAP
