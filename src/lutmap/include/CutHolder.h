#ifndef CUTHOLDER_H
#define CUTHOLDER_H

/// @file CutHolder.h
/// @brief CutHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCutOp.h"
#include "Cut.h"
#include "CutList.h"
#include "CutMgr.h"
#include "SbjNode.h"


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
  cut_list(const SbjNode* node) const;

  /// @brief 現在のカットを列挙したときのカットサイズを返す．
  int
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
  void
  all_init(const SbjGraph& sbjgraph,
	   int limit) override;

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  void
  node_init(const SbjNode* node,
	    int cur_pos) override;

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  void
  found(const SbjNode* root) override;

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  void
  found(const SbjNode* root,
	int ni,
	const SbjNode* inputs[]) override;

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  void
  node_end(const SbjNode* node,
	   int cur_pos,
	   int ncuts) override;

  /// @brief 処理の最後に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @note sbjgraph, limit, mode は 対となる all_init で
  /// 用いられたものと同じものが与えられる．
  void
  all_end(const SbjGraph& sbjgraph,
	  int limit) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットを管理するオブジェクト
  CutMgr mMgr;

  // カットサイズ
  int mLimit;

  // 各ノードのカットのリスト
  CutList* mCutList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief node を根とするカットのリストを取り出す．
inline
const CutList&
CutHolder::cut_list(const SbjNode* node) const
{
  return mCutList[node->id()];
}

// @brief 現在のカットを列挙したときのカットサイズを返す．
inline
int
CutHolder::limit() const
{
  return mLimit;
}

END_NAMESPACE_YM_LUTMAP

#endif // CUTHOLDER_H
