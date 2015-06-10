#ifndef EQEDGE_H
#define EQEDGE_H

/// @file EqEdge.h
/// @brief EqEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_NLINK

class EqNode;

//////////////////////////////////////////////////////////////////////
/// @class EqEdge EqEdge.h "EqEdge.h"
/// @brief equality logic の変数間の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class EqEdge
{
public:

  /// @brief コンストラクタ
  /// @param[in] node1, node2 両端のノード
  /// @param[in] var 対応する命題変数
  EqEdge(EqNode* node1,
	 EqNode* node2,
	 VarId var);

  /// @brief デストラクタ
  ~EqEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード1を返す．
  EqNode*
  node1() const;

  /// @brief ノード2を返す．
  EqNode*
  node2() const;

  /// @brief 反対側のノードを返す．
  /// @param[in] node 対象のノード
  EqNode*
  alt_node(EqNode* node) const;

  /// @brief 命題変数を返す．
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

  // ノード1
  EqNode* mNode1;

  // ノード2
  EqNode* mNode2;

  // 変数
  VarId mVar;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node1, node2 両端のノード
// @param[in] var 対応する命題変数
inline
EqEdge::EqEdge(EqNode* node1,
	       EqNode* node2,
	       VarId var) :
  mNode1(node1),
  mNode2(node2),
  mVar(var)
{
}

// @brief デストラクタ
inline
EqEdge::~EqEdge()
{
}

// @brief ノード1を返す．
inline
EqNode*
EqEdge::node1() const
{
  return mNode1;
}

// @brief ノード2を返す．
inline
EqNode*
EqEdge::node2() const
{
  return mNode2;
}

// @brief 反対側のノードを返す．
// @param[in] node 対象のノード
inline
EqNode*
EqEdge::alt_node(EqNode* node) const
{
  if ( mNode1 == node ) {
    return mNode2;
  }
  else {
    return mNode1;
  }
}

// @brief 命題変数を返す．
inline
VarId
EqEdge::var() const
{
  return mVar;
}

END_NAMESPACE_YM_NLINK

#endif // EQEDGE_H
