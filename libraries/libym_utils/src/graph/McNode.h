#ifndef MCNODE_H
#define MCNODE_H

/// @file McNode.h
/// @brief McNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class McNode McNode.h "McNode.h"
/// @brief max_clique で用いられるクラス
//////////////////////////////////////////////////////////////////////
class McNode
{
  friend class McNodeHeap;

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @param[in] id ノード番号
  void
  set(ymuint id);

  /// @brief 隣接ノードの情報を設定する．
  void
  set_adj_link(ymuint adj_num,
	       McNode** adj_link);

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
  McNode*
  adj_node(ymuint pos) const;

  /// @brief 有効な隣接ノード数を返す．
  ymuint
  adj_num() const;

  /// @brief adj_num を１減らす
  void
  dec_adj_num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  ymuint32 mId;

  // 隣接するノードのポインタ配列
  McNode** mAdjLink;

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
McNode::set(ymuint id)
{
  mId = id;
  mAdjLink = NULL;
  mAdjSize = 0;
  mNum = 0;
  mHeapIdx = 0;
}

// @brief 隣接ノードの情報を設定する．
inline
void
McNode::set_adj_link(ymuint adj_num,
		     McNode** adj_link)
{
  mAdjLink = adj_link;
  mAdjSize = adj_num;
  mNum = adj_num;
}

// @brief ノード番号を返す．
inline
ymuint
McNode::id() const
{
  return mId;
}

// @brief 削除済みフラグを返す．
inline
bool
McNode::deleted() const
{
  return mHeapIdx == 0;
}

// @brief 隣接するノード数を返す．
inline
ymuint
McNode::adj_size() const
{
  return mAdjSize;
}

// @brief 隣接するノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
inline
McNode*
McNode::adj_node(ymuint pos) const
{
  return mAdjLink[pos];
}

// @brief 有効な隣接ノード数を返す．
inline
ymuint
McNode::adj_num() const
{
  return mNum;
}

// @brief adj_num を１減らす
inline
void
McNode::dec_adj_num()
{
  -- mNum;
}

END_NAMESPACE_YM

#endif // MCNODE_H
