#ifndef BTGEDGE_H
#define BTGEDGE_H

/// @file BtgEdge.h
/// @brief BtgEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "btg_nsdef.h"


BEGIN_NAMESPACE_YM_BTG

//////////////////////////////////////////////////////////////////////
/// @class BtgEdge BtgEdge.h "BtgEdge.h"
/// @brief BtgMatch の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class BtgEdge
{
  friend class BtgMatchImpl;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node1 節点グループ1 側のノード
  /// @param[in] node2 節点グループ2 側のノード
  /// @param[in] weight 重み
  BtgEdge(ymuint id,
	  BtgNode* node1,
	  BtgNode* node2,
	  ymuint weight);

  /// @brief デストラクタ
  ~BtgEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 節点グループ1 側のノードを返す．
  BtgNode*
  node1() const;

  /// @brief 節点グループ2 側のノードを返す．
  BtgNode*
  node2() const;

  /// @brief 重みを返す．
  ymuint
  weight() const;

  /// @brief 節点グループ1 の次の枝を返す．
  BtgEdge*
  link1() const;

  /// @brief 節点グループ2 の次の枝を返す．
  BtgEdge*
  link2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 節点グループ1 側のノード
  BtgNode* mNode1;

  // 節点グループ2 側のノード
  BtgNode* mNode2;

  // 重み
  ymuint32 mWeight;

  // mNode1 用のリンクポインタ
  BtgEdge* mLink1;

  // mNode2 用のリンクポインタ
  BtgEdge* mLink2;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node1 節点グループ1 側のノード
// @param[in] node2 節点グループ2 側のノード
// @param[in] weight 重み
inline
BtgEdge::BtgEdge(ymuint id,
		 BtgNode* node1,
		 BtgNode* node2,
		 ymuint weight) :
  mId(id),
  mNode1(node1),
  mNode2(node2),
  mWeight(weight),
  mLink1(NULL),
  mLink2(NULL)
{
}

// @brief デストラクタ
inline
BtgEdge::~BtgEdge()
{
}

// @brief ID番号を返す．
inline
ymuint
BtgEdge::id() const
{
  return mId;
}

// @brief 節点グループ1 側のノードを返す．
inline
BtgNode*
BtgEdge::node1() const
{
  return mNode1;
}

// @brief 節点グループ2 側のノードを返す．
inline
BtgNode*
BtgEdge::node2() const
{
  return mNode2;
}

// @brief 重みを返す．
inline
ymuint
BtgEdge::weight() const
{
  return mWeight;
}

// @brief 節点グループ1 の次の枝を返す．
inline
BtgEdge*
BtgEdge::link1() const
{
  return mLink1;
}

// @brief 節点グループ2 の次の枝を返す．
inline
BtgEdge*
BtgEdge::link2() const
{
  return mLink2;
}

END_NAMESPACE_YM_BTG

#endif // BTGEDGE_H
