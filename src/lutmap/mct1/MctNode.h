#ifndef MCTNODE_H
#define MCTNODE_H

/// @file MctNode.h
/// @brief MctNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "mct1_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP_MCT1

//////////////////////////////////////////////////////////////////////
/// @class MctNode MctNode.h "MctNode.h"
/// @brief MCT で用いる探索木のノード
//////////////////////////////////////////////////////////////////////
class MctNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のノード
  /// @param[in] root 選択されたカットの根のノード
  /// @param[in] cand_list カット候補のリスト
  MctNode(MctNode* parent,
	  const SbjNode* root,
	  const vector<const SbjNode*>& cand_list);

  /// @brief デストラクタ
  ~MctNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のノードを返す．
  MctNode*
  parent() const;

  /// @brief この局面に移動する原因となっているカットの根のノードを返す．
  const SbjNode*
  cut_root() const;

  /// @brief 子供のノードが展開されていたら true を返す．
  bool
  is_expanded() const;

  /// @brief 未展開の子供を取り出す．
  ///
  /// is_expanded() == false のときのみ意味を持つ．
  /// 取り出されたノードは展開済みとなる．
  const SbjNode*
  expand_child();

  /// @brief 子供の数を返す．
  ymuint
  child_num() const;

  /// @brief 子供ノードを取り出す．
  MctNode*
  best_child() const;

  /// @brief 子供ノードを先頭に追加する．
  void
  insert_child(MctNode* child);

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

  // この局面で選ばれたカット(の根のノード)
  const SbjNode* mRoot;

  // 未展開の子供のリスト
  vector<const SbjNode*> mUnexpandedList;

  // 未展開の子供のインデックス
  ymuint mNextPos;

  // 子ノードのリスト
  vector<MctNode*> mChildList;

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

// @brief この局面に移動する原因となっているカットの根のノードを返す．
inline
const SbjNode*
MctNode::cut_root() const
{
  return mRoot;
}

// @brief 子供のノードが展開されていたら true を返す．
inline
bool
MctNode::is_expanded() const
{
  return mNextPos == mUnexpandedList.size();
}

// @brief 子供の数を返す．
inline
ymuint
MctNode::child_num() const
{
  return mChildList.size();
}

// @brief 子供ノードを取り出す．
inline
MctNode*
MctNode::best_child() const
{
  return mChildList.back();
}

// @brief 子供ノードを先頭に追加する．
inline
void
MctNode::insert_child(MctNode* child)
{
  mChildList.push_back(child);
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
  return mMean + 0.3 * sqrt(2 * n_all_ln / mNum);
}

END_NAMESPACE_YM_LUTMAP_MCT1

#endif // MCTNODE_H
