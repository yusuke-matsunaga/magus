#ifndef NLNODE_H
#define NLNODE_H

/// @file NlGraph.h
/// @brief NlGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlNode NlGraph.h "NlGraph.h"
/// @brief NlGraph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class NlNode
{
  friend class NlGraph;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] x, y 座標
  NlNode(ymuint id,
	 ymuint x,
	 ymuint y);

  /// @brief デストラクタ
  ~NlNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief X座標を得る．
  ymuint
  x() const;

  /// @brief Y座標を得る．
  ymuint
  y() const;

  /// @brief 端点番号を得る．
  ///
  /// 端点でない場合は 0 を返す．
  ymuint
  terminal_id() const;

  /// @brief 接続している枝番号のリストを返す．
  const vector<const NlEdge*>&
  edge_list() const;

  /// @brief 左の枝を返す．
  ///
  /// なければ NULL を返す．
  const NlEdge*
  left_edge() const;

  /// @brief 右の枝を返す．
  ///
  /// なければ NULL を返す．
  const NlEdge*
  right_edge() const;

  /// @brief 上の枝を返す．
  ///
  /// なければ NULL を返す．
  const NlEdge*
  upper_edge() const;

  /// @brief 下の枝を返す．
  ///
  /// なければ NULL を返す．
  const NlEdge*
  lower_edge() const;

  /// @brief 内容を表す文字列を返す．
  string
  str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // X座標
  ymuint mX;

  // Y座標
  ymuint mY;

  // 端点番号
  // 0 で無印
  ymuint mTermId;

  // 接続する枝番号のリスト
  vector<const NlEdge*> mEdgeList;

  // 左の枝
  const NlEdge* mLeftEdge;

  // 右の枝
  const NlEdge* mRightEdge;

  // 上の枝
  const NlEdge* mUpperEdge;

  // 下の枝
  const NlEdge* mLowerEdge;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] x, y 座標
inline
NlNode::NlNode(ymuint id,
	       ymuint x,
	       ymuint y) :
  mId(id),
  mX(x),
  mY(y)
{
  mTermId = 0;
  mLeftEdge = NULL;
  mRightEdge = NULL;
  mUpperEdge = NULL;
  mLowerEdge = NULL;
}

// @brief デストラクタ
inline
NlNode::~NlNode()
{
}

// @brief ID番号を返す．
inline
ymuint
NlNode::id() const
{
  return mId;
}

// @brief X座標を得る．
inline
ymuint
NlNode::x() const
{
  return mX;
}

// @brief Y座標を得る．
inline
ymuint
NlNode::y() const
{
  return mY;
}

// @brief 端点番号を得る．
//
// 端点でない場合は 0 を返す．
inline
ymuint
NlNode::terminal_id() const
{
  return mTermId;
}

// @brief 接続している枝番号のリストを返す．
inline
const vector<const NlEdge*>&
NlNode::edge_list() const
{
  return mEdgeList;
}

// @brief 左の枝を返す．
//
// なければ NULL を返す．
inline
const NlEdge*
NlNode::left_edge() const
{
  return mLeftEdge;
}

// @brief 右の枝を返す．
//
// なければ NULL を返す．
inline
const NlEdge*
NlNode::right_edge() const
{
  return mRightEdge;
}

// @brief 上の枝を返す．
//
// なければ NULL を返す．
inline
const NlEdge*
NlNode::upper_edge() const
{
  return mUpperEdge;
}

// @brief 下の枝を返す．
//
// なければ NULL を返す．
inline
const NlEdge*
NlNode::lower_edge() const
{
  return mLowerEdge;
}

// @brief 内容を表す文字列を返す．
inline
string
NlNode::str() const
{
  ostringstream buf;
  buf << "(" << x() << ", " << y() << ")";
  if ( terminal_id() > 0 ) {
    buf << "[" << terminal_id() << "]";
  }
  return buf.str();
}

END_NAMESPACE_YM_NLINK

#endif // NLNODE_H
