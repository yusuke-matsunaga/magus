#ifndef ZDDOP_H
#define ZDDOP_H

/// @file ZddOp.h
/// @brief ZddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/zdd_nsdef.h"
#include "ZddMgrImpl.h"
#include "ZddEdge.h"
#include "ZddNode.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddOp ZddOp.h "ZddOp.h"
/// @brief ZDD の演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  ZddOp(ZddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~ZddOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep() = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左右の枝が同じ場合にはその枝自身を返し，それ以外の場合には，
  /// @return 与えられた枝とインデックスを持つノードを返す．
  ZddEdge
  new_node(ymuint level,
	   ZddEdge l,
	   ZddEdge h);

  /// @brief split() の下請け関数
  /// @param[in] top_level 根のレベル
  /// @param[in] level レベル
  /// @param[in] e 枝
  /// @param[in] node ノード
  /// @param[out] e_0 0枝
  /// @param[out] e_1 1枝
  static
  void
  split1(ymuint top_level,
	 ymuint level,
	 ZddEdge e,
	 ZddNode* node,
	 ZddEdge& e_0,
	 ZddEdge& e_1);

  /// @brief マネージャを返す．
  ZddMgrImpl*
  mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgrImpl* mMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
// @param[in] mgr マネージャ
inline
ZddOp::ZddOp(ZddMgrImpl* mgr) :
  mMgr(mgr)
{
  mMgr->mOpList.push_back(this);
}

// @brief 左右の枝が同じ場合にはその枝自身を返し，それ以外の場合には，
// @return 与えられた枝とインデックスを持つノードを返す．
inline
ZddEdge
ZddOp::new_node(ymuint level,
		ZddEdge l,
		ZddEdge h)
{
  return mMgr->new_node(level, l, h);
}

// @brief split() の下請け関数
// @param[in] top_level 根のレベル
// @param[in] level レベル
// @param[in] e 枝
// @param[in] node ノード
// @param[out] e_0 0枝
// @param[out] e_1 1枝
inline
void
ZddOp::split1(ymuint top_level,
	      ymuint level,
	      ZddEdge e,
	      ZddNode* node,
	      ZddEdge& e_0,
	      ZddEdge& e_1)
{
  if ( top_level == level ) {
    e_0 = node->edge0();
    e_1 = node->edge1();
    e_0.add_zattr(e.zattr());
  }
  else {
    e_0 = e;
    e_1 = ZddEdge::make_zero();
  }
}

// @brief マネージャを返す．
inline
ZddMgrImpl*
ZddOp::mgr() const
{
  return mMgr;
}

END_NAMESPACE_YM_ZDD

#endif // ZDDOP_H
