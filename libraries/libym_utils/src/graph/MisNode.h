#ifndef MISNODE_H
#define MISNODE_H

/// @file MisNode.h
/// @brief MisNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MisNode MisNode.h "MisNode.h"
/// @brief max_independent_set で用いられるクラス
//////////////////////////////////////////////////////////////////////
class MisNode
{
  friend class MisNodeHeap;

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @param[in] id ノード番号
  void
  set(ymuint id);

  /// @brief 隣接ノードの情報を設定する．
  /// @param[in] adj_num 隣接するノードの数
  /// @param[in] adj_link 隣接するノードのリスト(ポインタ配列)
  void
  set_adj_link(ymuint adj_num,
	       MisNode** adj_link);

  /// @brief ノード番号を返す．
  ymuint
  id() const;

  /// @brief 削除済みフラグを返す．
  bool
  deleted() const;

  /// @brief 隣接するノード数を返す．
  ymuint
  adj_size() const;

  /// @brief 隣接するノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
  MisNode*
  adj_node(ymuint pos) const;

  /// @brief 有効な隣接ノード数を返す．
  ymuint
  adj_num() const;

  /// @brief adj_num を1減らす
  void
  dec_adj_num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  ymuint32 mId;

  // 隣接するノードのポインタ配列
  MisNode** mAdjLink;

  // mAdjLink のサイズ
  ymuint32 mAdjSize;

  // mAdjLink 中の有効な要素数
  ymuint32 mNum;

  // ヒープ上のインデックス
  ymuint32 mHeapIdx;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 内容を初期化する．
// @param[in] id ノード番号
inline
void
MisNode::set(ymuint id)
{
  mId = id;
  mAdjLink = NULL;
  mAdjSize = 0;
  mNum = 0;
  mHeapIdx = 0;
}

// @brief 隣接ノードの情報を設定する．
// @param[in] adj_num 隣接するノードの数
// @param[in] adj_link 隣接するノードのリスト(ポインタ配列)
inline
void
MisNode::set_adj_link(ymuint adj_num,
		      MisNode** adj_link)
{
  mAdjLink = adj_link;
  mAdjSize = adj_num;
  mNum = adj_num;
}

// @brief ノード番号を返す．
inline
ymuint
MisNode::id() const
{
  return mId;
}

// @brief 削除済みフラグを返す．
inline
bool
MisNode::deleted() const
{
  return mHeapIdx == 0;
}

// @brief 隣接するノード数を返す．
inline
ymuint
MisNode::adj_size() const
{
  return mAdjSize;
}

// @brief 隣接するノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
inline
MisNode*
MisNode::adj_node(ymuint pos) const
{
  return mAdjLink[pos];
}

// @brief 有効な隣接ノード数を返す．
inline
ymuint
MisNode::adj_num() const
{
  return mNum;
}

// @brief adj_num を1減らす
inline
void
MisNode::dec_adj_num()
{
  -- mNum;
}

END_NAMESPACE_YM

#endif // MISNODE_H
