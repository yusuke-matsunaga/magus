#ifndef GCNODE_H
#define GCNODE_H

/// @file GcNode.h
/// @brief GcNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GcNode GcNode.h "GcNode.h"
/// @brief 彩色問題を表すグラフのノード
//////////////////////////////////////////////////////////////////////
class GcNode
{
  friend class GcMgr;

private:

  /// @brief コンストラクタ
  GcNode();

  /// @brief デストラクタ
  ~GcNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 隣接するノードのリストを得る．
  const vector<GcNode*>&
  link_list() const;

  /// @brief 隣接するノードの度数を返す．
  ymuint
  adj_dgree() const;

  /// @brief saturation degree を返す．
  /// @param[in] max_color 現時点の彩色数
  ymuint
  sat_dgree(ymuint max_color) const;

  /// @brief 色番号を得る．
  /// @note 0 は未彩色を表す．
  ymuint
  color() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 隣接するノードのリスト
  vector<GcNode*> mLinkList;

  // 色番号
  // 0 が未彩色
  ymuint32 mColor;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
GcNode::id() const
{
  return mId;
}

// @brief 隣接するノードのリストを得る．
inline
const vector<GcNode*>&
GcNode::link_list() const
{
  return mLinkList;
}

// @brief 隣接するノードの度数を返す．
inline
ymuint
GcNode::adj_dgree() const
{
  return mLinkList.size();
}

// @brief 色番号を得る．
// @note 0 は未彩色を表す．
inline
ymuint
GcNode::color() const
{
  return mColor;
}

END_NAMESPACE_YM_SATPG

#endif // GCNODE_H
