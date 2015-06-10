#ifndef NLEDGE_H
#define NLEDGE_H

/// @file NlEdge.h
/// @brief NlEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlEdge NlEdge.h "NlEdge.h"
/// @brief number link のグラフの枝を表すクラス
//////////////////////////////////////////////////////////////////////
class NlEdge
{
  friend class NlGraph;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] str デバッグ用の文字列
  NlEdge(ymuint id,
	 const string& str);

  /// @brief デストラクタ
  ~NlEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief ノードを返す．
  const NlNode*
  node1() const;

  /// @brief ノードを返す．
  const NlNode*
  node2() const;

  /// @brief 相方のノードを返す．
  /// @param[in] node ノード
  const NlNode*
  alt_node(const NlNode* node) const;

  /// @brief 内容を表す文字列を返す．
  string
  str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 接続するノード
  const NlNode* mNode1;

  // 接続するノード
  const NlNode* mNode2;

  // 内容を表す文字列
  string mStr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] str デバッグ用の文字列
inline
NlEdge::NlEdge(ymuint id,
	       const string& str) :
  mId(id),
  mStr(str)
{
  mNode1 = NULL;
  mNode2 = NULL;
}

// @brief デストラクタ
inline
NlEdge::~NlEdge()
{
}

// @brief ID番号を返す．
inline
ymuint
NlEdge::id() const
{
  return mId;
}

// @brief ノードを返す．
inline
const NlNode*
NlEdge::node1() const
{
  return mNode1;
}

// @brief ノードを返す．
inline
const NlNode*
NlEdge::node2() const
{
  return mNode2;
}

// @brief 相方のノードを返す．
// @param[in] node ノード
inline
const NlNode*
NlEdge::alt_node(const NlNode* node) const
{
  return (mNode1 == node) ? mNode2 : mNode1;
}

// @brief 内容を表す文字列を返す．
inline
string
NlEdge::str() const
{
  return mStr;
}

END_NAMESPACE_YM_NLINK

#endif // NLEDGE_H
