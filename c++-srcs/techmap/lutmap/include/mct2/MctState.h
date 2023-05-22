#ifndef MCTSTATE_H
#define MCTSTATE_H

/// @file MctState.h
/// @brief MctState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "mct2_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_LUTMAP_MCT2

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

  /// @brief ブロックノードを追加する．
  void
  add_block(const SbjNode* node);

  /// @brief 境界ノードのリストを得る．
  const vector<const SbjNode*>&
  boundary_list() const;

  /// @brief ブロックノードのリストを得る．
  const vector<const SbjNode*>&
  block_list() const;

  /// @brief 処理済みのファンアウトポイントのインデックスを得る．
  SizeType
  index() const;

  /// @brief インデックスを一つすすめる．
  void
  next_index();


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

  // ブロックノードのリスト
  vector<const SbjNode*> mBlockList;

  // 処理済みのファンアウトポイントのインデックス
  SizeType mIndex;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 境界ノードを追加する．
inline
void
MctState::add_boundary(const SbjNode* node)
{
  ASSERT_COND( node );
  mBoundaryList.push_back(node);
}

// @brief ブロックノードを追加する．
inline
void
MctState::add_block(const SbjNode* node)
{
  ASSERT_COND( node );
  mBlockList.push_back(node);
}

// @brief 境界ノードのリストを得る．
inline
const vector<const SbjNode*>&
MctState::boundary_list() const
{
  return mBoundaryList;
}

// @brief ブロックノードのリストを得る．
inline
const vector<const SbjNode*>&
MctState::block_list() const
{
  return mBlockList;
}

// @brief 処理済みのファンアウトポイントのインデックスを得る．
inline
SizeType
MctState::index() const
{
  return mIndex;
}

// @brief インデックスを一つすすめる．
inline
void
MctState::next_index()
{
  ++ mIndex;
}

END_NAMESPACE_LUTMAP_MCT2

#endif // MCTSTATE_H
