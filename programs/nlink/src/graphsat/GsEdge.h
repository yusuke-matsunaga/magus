#ifndef GSEDGE_H
#define GSEDGE_H

/// @file GsEdge.h
/// @brief GsEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_NLINK

class GsNode;

//////////////////////////////////////////////////////////////////////
/// @class GsEdge GsEdge.h "GsEdge.h"
/// @brief GraphSat 用の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class GsEdge
{
  friend class GsGraph;

public:

  /// @brief コンストラクタ
  GsEdge();

  /// @brief デストラクタ
  ~GsEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 1つめの節点を返す．
  GsNode*
  node1() const;

  /// @brief 2つめの節点を返す．
  GsNode*
  node2() const;

  /// @brief この枝が属するグラフを返す．
  GsGraph*
  graph() const;

  /// @brief 関連する変数番号を返す．
  VarId
  var() const;

  /// @brief パス上にあるとき true を返す．
  bool
  selected() const;

  /// @brief 'selected' フラグを消す．
  void
  clear_selected();

  /// @brief 'selected' フラグをつける．
  void
  set_selected();


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

  // 1つめの節点
  GsNode* mNode1;

  // 2つめの節点
  GsNode* mNode2;

  // この枝が属するグラフ
  GsGraph* mGraph;

  // 関連する変数番号
  VarId mVar;

  // パス上にあることを示すフラグ
  bool mSelected;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
GsEdge::GsEdge()
{
}

// @brief デストラクタ
inline
GsEdge::~GsEdge()
{
}

// @brief ID番号を返す．
inline
ymuint
GsEdge::id() const
{
  return mId;
}

// @brief 1つめの節点を返す．
inline
GsNode*
GsEdge::node1() const
{
  return mNode1;
}

// @brief 2つめの節点を返す．
inline
GsNode*
GsEdge::node2() const
{
  return mNode2;
}

// @brief この枝が属するグラフを返す．
inline
GsGraph*
GsEdge::graph() const
{
  return mGraph;
}

// @brief パス上にあるとき true を返す．
inline
bool
GsEdge::selected() const
{
  return mSelected;
}

// @brief 'selected' フラグを消す．
inline
void
GsEdge::clear_selected()
{
  mSelected = false;
}

// @brief 'selected' フラグをつける．
inline
void
GsEdge::set_selected()
{
  mSelected = true;
}

// @brief 関連する変数番号を返す．
inline
VarId
GsEdge::var() const
{
  return mVar;
}

END_NAMESPACE_YM_NLINK

#endif // GSEDGE_H
