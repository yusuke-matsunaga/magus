﻿
/// @file lutmap/EnumCutOp.cc
/// @brief EnumCutOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCutOp.h"
#include "EnumCut.h"

//#define DEBUG_ENUM_RECUR


BEGIN_NAMESPACE_YM_LUTMAP

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
// @param[in] 対象のサブジェクトグラフ
// @param[in] limit 入力数の制限
// @return 全 cut 数を返す．
int
EnumCutOp::enum_cut(const SbjGraph& sbjgraph,
		    int limit)
{
  EnumCut ec;
  return ec(sbjgraph, limit, this);
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::all_init(const SbjGraph& sbjgraph,
		    int limit)
{
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::node_init(const SbjNode* node,
		     int cur_pos)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
// @param[in] root 根のノード
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::found(const SbjNode* root)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
// @param[in] root 根のノード
// @param[in] ni 入力数
// @param[in] inputs 入力ノードの配列
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::found(const SbjNode* root,
		 int ni,
		 const SbjNode* inputs[])
{
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @param[in] ncuts 列挙されたカット数
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::node_end(const SbjNode* node,
		    int cur_pos,
		    int ncuts)
{
}

// @brief 処理の最後に呼ばれる関数
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::all_end(const SbjGraph& sbjgraph,
		   int limit)
{
}

END_NAMESPACE_YM_LUTMAP
