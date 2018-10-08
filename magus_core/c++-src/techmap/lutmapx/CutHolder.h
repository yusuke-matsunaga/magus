﻿#ifndef LIBYM_TECHMAP_LUTMAP_CUTHOLDER_H
#define LIBYM_TECHMAP_LUTMAP_CUTHOLDER_H

/// @file libym_techmap/lutmap/CutHolder.h
/// @brief CutHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutHolder.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCutOp.h"
#include "Cut.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class CutHolder
/// @brief 各ノードの cut を保持するクラス
//////////////////////////////////////////////////////////////////////
class CutHolder :
  public EnumCutOp
{
public:

  /// @brief コンストラクタ
  CutHolder();

  /// @brief デストラクタ
  virtual
  ~CutHolder();


public:

  /// @brief node を根とするカットのリストを取り出す．
  const CutList&
  cut_list(const BdnNode* node) const;

  /// @brief 現在のカットを列挙したときのカットサイズを返す．
  ymuint
  limit() const;

  /// @brief 保持しているカットのリストを削除する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // EnumCutOp の継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  virtual
  void
  all_init(const BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  virtual
  void
  node_init(const BdnNode* node,
	    ymuint cur_pos);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  virtual
  void
  found(const BdnNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  virtual
  void
  found(const BdnNode* root,
	ymuint ni,
	const BdnNode* inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  virtual
  void
  node_end(const BdnNode* node,
	   ymuint cur_pos,
	   ymuint ncuts);

  /// @brief 処理の最後に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @note sbjgraph, limit, mode は 対となる all_init で
  /// 用いられたものと同じものが与えられる．
  virtual
  void
  all_end(const BdnMgr& sbjgraph,
	  ymuint limit);


private:

  // カットの領域を確保する．
  Cut*
  alloc_cut(ymuint ni);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットサイズ
  ymuint32 mLimit;

  // 各ノードのカットのリスト
  CutList* mCutList;

  static
  const ymuint kPageSize = 8192;

  char* mCurChunk;

  ymuint32 mLast;

  vector<char*> mChunk;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief node を根とするカットのリストを取り出す．
inline
const CutList&
CutHolder::cut_list(const BdnNode* node) const
{
  return mCutList[node->id()];
}

// @brief 現在のカットを列挙したときのカットサイズを返す．
inline
ymuint
CutHolder::limit() const
{
  return mLimit;
}

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_TECHMAP_LUTMAP_CUTHOLDER_H
