#ifndef MCTSTATE_H
#define MCTSTATE_H

/// @file MctState.h
/// @brief MctState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap3_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP3

//////////////////////////////////////////////////////////////////////
/// @class MctState MctState.h "MctState.h"
/// @brief MCT 探索の状態を表すクラス
//////////////////////////////////////////////////////////////////////
class MctState
{
public:

  /// @brief コンストラクタ
  /// @param[in] sbjgraph サブジェクトグラフ
  MctState(const SbjGraph& sbjgraph);

  /// @brief デストラクタ
  ~MctState();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief 境界ノードを追加する．
  void
  add_boundary(const SbjNode* node);

  /// @brief 境界ノードのリストを得る．
  const vector<const SbjNode*>&
  boundary_list() const;

  /// @brief 処理済みのファンアウトポイントのインデックスを得る．
  ymuint
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 境界ノードの初期値
  // (外部出力ノードのリスト)
  vector<const SbjNode*> mOutputList;

  // 境界ノードのリスト
  vector<const SbjNode*> mBoundaryList;

  // 境界ノードのマークの配列
  // サイズは max_node_id
  vector<bool> mBoundaryMark;

  // 処理済みのファンアウトポイントのインデックス
  ymuint mIndex;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 境界ノードを追加する．
inline
void
MctState::add_boundary(const SbjNode* node)
{
  if ( !mBoundaryMark[node->id()] ) {
    mBoundaryMark[node->id()] = true;
    mBoundaryList.push_back(node);
  }
}

// @brief 境界ノードのリストを得る．
inline
const vector<const SbjNode*>&
MctState::boundary_list() const
{
  return mBoundaryList;
}

// @brief 処理済みのファンアウトポイントのインデックスを得る．
inline
ymuint
MctState::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_LUTMAP3

#endif // MCTSTATE_H
