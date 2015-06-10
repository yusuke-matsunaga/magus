#ifndef FRGEDGE_H
#define FRGEDGE_H

/// @file FrgEdge.h
/// @brief FrgEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

class FrgNode;

//////////////////////////////////////////////////////////////////////
/// @class FrgEdge FrgEdge.h "FrgEdge.h"
/// @brief 問題のグラフの枝を表すクラス
//////////////////////////////////////////////////////////////////////
class FrgEdge
{
  friend class FrgGraph;

public:

  /// @brief コンストラクタ
  FrgEdge();

  /// @brief デストラクタ
  ~FrgEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 一方の節点を返す．
  FrgNode*
  node1() const;

  /// @brief 他方の節点を返す．
  FrgNode*
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

  // 一方の節点
  FrgNode* mNode1;

  // もう一方の節点
  FrgNode* mNode2;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FrgEdge::FrgEdge()
{
}

// @brief デストラクタ
inline
FrgEdge::~FrgEdge()
{
}

// @brief ID番号を返す．
inline
ymuint
FrgEdge::id() const
{
  return mId;
}

// @brief 一方の節点を返す．
inline
FrgNode*
FrgEdge::node1() const
{
  return mNode1;
}

// @brief 他方の節点を返す．
inline
FrgNode*
FrgEdge::node2() const
{
  return mNode2;
}

END_NAMESPACE_YM_NLINK

#endif // FRGEDGE_H
