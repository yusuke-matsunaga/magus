#ifndef FRNODE_H
#define FRNODE_H

/// @file FrNode.h
/// @brief FrNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "FrontierInfo.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class FrNode FrNode.h "FrNode.h"
/// @brief フロンティア法のノード
//////////////////////////////////////////////////////////////////////
class FrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] edge_id
  FrNode(ymuint edge_id);

  /// @brief デストラクタ
  ~FrNode();

  /// @brief 0終端ノードを作る．
  static
  FrNode*
  const0();

  /// @brief 1終端ノードを作る．
  static
  FrNode*
  const1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 0終端ノードの時 true を返す．
  bool
  is_const0() const;

  /// @brief 1終端ノードの時 true を返す．
  bool
  is_const1() const;

  /// @brief 枝番号を返す．
  ymuint
  edge_id() const;

  /// @brief フロンティア情報を返す．
  const FrontierInfo&
  frontier_info() const;

  /// @brief フロンティア情報を返す．
  FrontierInfo&
  frontier_info();

  /// @brief 0枝の先のノードを返す．
  FrNode*
  edge0() const;

  /// @brief 1枝の先のノードを返す．
  FrNode*
  edge1() const;

  /// @brif 0枝をセットする．
  void
  set_edge0(FrNode* node);

  /// @brif 1枝をセットする．
  void
  set_edge1(FrNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数フラグ
  // 0: 0終端
  // 1: 1終端
  // 2: それ以外
  ymuint mConstFlag;

  // 枝番号
  ymuint mEdgeId;

  // フロンティア情報
  FrontierInfo mFrontierInfo;

  // 0枝
  FrNode* mEdge0;

  // 1枝
  FrNode* mEdge1;

};

END_NAMESPACE_YM_NLINK

#endif // FRNODE_H
