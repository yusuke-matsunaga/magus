#ifndef CUTHOLDER_H
#define CUTHOLDER_H

/// @file CutHolder.h
/// @brief CutHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "EnumCutOp.h"
#include "Cut.h"
#include "CutList.h"
#include "CutMgr.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP

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
  cut_list(
    const SbjNode* node ///< [in] ノード
  ) const
  {
    return mCutList[node->id()];
  }

  /// @brief 現在のカットを列挙したときのカットサイズを返す．
  SizeType
  limit() const
  {
    return mLimit;
  }

  /// @brief 保持しているカットのリストを削除する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // EnumCutOp の継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  void
  all_init(
    const SbjGraph& sbjgraph, ///< [in] 対象のサブジェクトグラフ
    SizeType limit            ///< [in] カットサイズ
  ) override;

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  void
  node_init(
    const SbjNode* node, ///< [in] 根のノード
    SizeType cur_pos     ///< [in] node の処理順
  ) override;

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  void
  found(
    const SbjNode* root ///< [in] 根のノード
  ) override;

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  void
  found(
    const SbjNode* root,     ///< [in] 根のノード
    SizeType ni,             ///< [in] 入力数
    const SbjNode* inputs[]  ///< [in] 入力ノードの配列
  ) override;

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  void
  node_end(
    const SbjNode* node, ///< [in] 根のノード
    SizeType cur_pos,    ///< [in] node の処理順
    SizeType ncuts       ///< [in] 列挙されたカット数
  ) override;

  /// @brief 処理の最後に呼ばれる関数
  ///
  /// sbjgraph, limit, mode は 対となる all_init で
  /// 用いられたものと同じものが与えられる．
  void
  all_end(
    const SbjGraph& sbjgraph, ///< [in] 対象のサブジェクトグラフ
    SizeType limit            ///< [in] カットサイズ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットを管理するオブジェクト
  CutMgr mMgr;

  // カットサイズ
  SizeType mLimit;

  // 各ノードのカットのリスト
  CutList* mCutList;

};

END_NAMESPACE_LUTMAP

#endif // CUTHOLDER_H
