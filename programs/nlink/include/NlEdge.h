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

class NlNode;

//////////////////////////////////////////////////////////////////////
/// @class NlEdge NlEdge.h "NlEdge.h"
/// @brief number link のグラフの枝を表すクラス
//////////////////////////////////////////////////////////////////////
class NlEdge
{
  friend class NlGraph;

public:

  /// @brief コンストラクタ
  NlEdge();

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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
NlEdge::NlEdge()
{
  mId = 0;
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

END_NAMESPACE_YM_NLINK

#endif // NLEDGE_H
