#ifndef MCTNODE_H
#define MCTNODE_H

/// @file MctNode.h
/// @brief MctNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap3_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP3

//////////////////////////////////////////////////////////////////////
/// @class MctNode MctNode.h "MctNode.h"
/// @brief MCT で用いる探索木のノード
//////////////////////////////////////////////////////////////////////
class MctNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のノード
  /// @param[in] index ファンアウトノードのインデックス
  /// @param[in] select 境界ノードとして選ぶ時 true にするフラグ
  MctNode(MctNode* parent,
	  ymuint index,
	  bool select);

  /// @brief デストラクタ
  ~MctNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のノードを返す．
  MctNode*
  parent() const;

  /// @brief ファンアウトノードのインデックスを返す．
  ymuint
  index() const;

  /// @brief ファンアウトノードを境界ノードとして選ぶ時 true を返す．
  bool
  is_selected() const;

  /// @brief 子供のノードが展開されていたら true を返す．
  bool
  is_expanded() const;

  /// @brief 未展開の子供を取り出す．
  ///
  /// is_expanded() == false のときのみ意味を持つ．
  /// 取り出されたノードは展開済みとなる．
  MctNode*
  expand_child();

  /// @brief 子供ノードを取り出す．
  MctNode*
  best_child() const;

  /// @brief 子供ノードを並び替える．
  /// @param[in] n_all_ln トータルの試行回数の ln
  void
  reorder(double n_all_ln);

  /// @brief 評価値の更新を行う．
  /// @param[in] val 評価値
  void
  update(double val);

  /// @brief UCB1 値を返す．
  /// @param[in] n_all_ln トータルの試行回数の ln
  double
  UCB1(double n_all_ln) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MctNode* mParent;

  // インデックス
  ymuint mIndex;

  // 境界として選ばれるかどうか
  bool mSelected;

  // 未展開の子供の数
  ymuint mUnexpandedNum;

  // 子ノードのリスト
  MctNode* mChildList[2];

  // 評価値の総和
  double mSum;

  // 試行回数
  ymuint mNum;

  // 現在の期待値 = mSum / mNum
  double mMean;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親のノードを返す．
inline
MctNode*
MctNode::parent() const
{
  return mParent;
}

// @brief ファンアウトノードのインデックスを返す．
inline
ymuint
MctNode::index() const
{
  return mIndex;
}

// @brief ファンアウトノードを境界ノードとして選ぶ時 true を返す．
inline
bool
MctNode::is_selected() const
{
  return mSelected;
}

// @brief 子供のノードが展開されていたら true を返す．
inline
bool
MctNode::is_expanded() const
{
  return mUnexpandedNum == 0;
}

// @brief 子供ノードを取り出す．
inline
MctNode*
MctNode::best_child() const
{
  return mChildList[0];
}

// @brief 評価値の更新を行う．
// @param[in] val 評価値
inline
void
MctNode::update(double val)
{
  mSum += val;
  ++ mNum;
  mMean = mSum / mNum;
}

// @brief UCB1 値を返す．
// @param[in] n_all トータルの試行回数の ln
inline
double
MctNode::UCB1(double n_all_ln) const
{
  return mMean + sqrt(2 * n_all_ln / mNum);
}

END_NAMESPACE_YM_LUTMAP3

#endif // MCTNODE_H
