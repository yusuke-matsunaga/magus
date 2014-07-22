
/// @file libym_techmap/lutmap/EnumCutOp.cc
/// @brief EnumCutOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EnumCutOp.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCutOp.h"
#include "EnumCut.h"
#include "networks/BdnMgr.h"

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
ymuint
EnumCutOp::enum_cut(const BdnMgr& sbjgraph,
		    ymuint limit)
{
  EnumCut ec;
  return ec(sbjgraph, limit, this);
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::all_init(const BdnMgr& sbjgraph,
		    ymuint limit)
{
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::node_init(const BdnNode* node,
		     ymuint cur_pos)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
// @param[in] root 根のノード
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::found(const BdnNode* root)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
// @param[in] root 根のノード
// @param[in] ni 入力数
// @param[in] inputs 入力ノードの配列
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::found(const BdnNode* root,
		 ymuint ni,
		 const BdnNode* inputs[])
{
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @param[in] ncuts 列挙されたカット数
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::node_end(const BdnNode* node,
		    ymuint cur_pos,
		    ymuint ncuts)
{
}

// @brief 処理の最後に呼ばれる関数
// @note デフォルトの実装ではなにもしない．
void
EnumCutOp::all_end(const BdnMgr& sbjgraph,
		   ymuint limit)
{
}

END_NAMESPACE_YM_LUTMAP
