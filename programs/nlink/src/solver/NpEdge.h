#ifndef NPEDGE_H
#define NPEDGE_H

/// @file NpEdge.h
/// @brief NpEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NpEdge NpEdge.h "NpEdge.h"
/// @brief nonpolar equality graph の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class NpEdge
{
  friend class NpGraph;

private:

  /// @brief コンストラクタ
  /// @param[in] node1, node2 両端のノード
  /// @param[in] var 対応する変数番号
  NpEdge(NpNode* node1,
	 NpNode* node2,
	 VarId var);

  /// @brief デストラクタ
  ~NpEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 一方のノードを返す．
  NpNode*
  node1() const;

  /// @brief もう一方のノードを返す．
  NpNode*
  node2() const;

  /// @brief 変数番号を返す．
  VarId
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一方のノード
  NpNode* mNode1;

  // もう一方のノード
  NpNode* mNode2;

  // 変数番号
  VarId mVar;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node1, node2 両端のノード
// @param[in] var 対応する変数番号
inline
NpEdge::NpEdge(NpNode* node1,
	       NpNode* node2,
	       VarId var) :
  mNode1(node1),
  mNode2(node2),
  mVar(var)
{
}

// @brief デストラクタ
inline
NpEdge::~NpEdge()
{
}

// @brief 一方のノードを返す．
inline
NpNode*
NpEdge::node1() const
{
  return mNode1;
}

// @brief もう一方のノードを返す．
inline
NpNode*
NpEdge::node2() const
{
  return mNode2;
}

// @brief 変数番号を返す．
inline
VarId
NpEdge::var() const
{
  return mVar;
}

END_NAMESPACE_YM_NLINK

#endif // NPEDGE_H
