#ifndef BDDOP_H
#define BDDOP_H

/// @file BddOp.h
/// @brief BddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/bdd_nsdef.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddOp BddOp.h "BddOp.h"
/// @brief BDD の演算を表すクラス
//////////////////////////////////////////////////////////////////////
class BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  BddOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~BddOp() { }


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
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた枝とインデックスを持つノードを返す．
  /// @param[in] level ノードのレベル
  /// @param[in] l 0枝
  /// @param[in] h 1枝
  BddEdge
  new_node(ymuint level,
	   BddEdge l,
	   BddEdge h);

  /// @brief idx が top に等しいときには e の子供を e_0, e_1 にセットする．
  /// 等しくなければ e をセットする．
  static
  void
  split1(ymuint top,
	 ymuint level,
	 BddEdge e,
	 const BddNode* vp,
	 tPol pol,
	 BddEdge& e_0,
	 BddEdge& e_1);

  /// @brief マネージャを返す．
  BddMgrImpl*
  mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgrImpl* mMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
inline
BddOp::BddOp(BddMgrImpl* mgr) :
  mMgr(mgr)
{
  mMgr->mOpList.push_back(this);
}

// @brief 与えられた枝とインデックスを持つノードを返す．
// @param[in] level ノードのレベル
// @param[in] l 0枝
// @param[in] h 1枝
inline
BddEdge
BddOp::new_node(ymuint level,
		BddEdge l,
		BddEdge h)
{
  return mMgr->new_node(level, l, h);
}

// idx が top に等しいときには e の子供を e_0, e_1 にセットする．
// 等しくなければ e をセットする．
inline
void
BddOp::split1(ymuint top,
	      ymuint level,
	      BddEdge e,
	      const BddNode* vp,
	      tPol pol,
	      BddEdge& e_0,
	      BddEdge& e_1)
{
  if ( level == top ) {
    e_0 = vp->edge0(pol);
    e_1 = vp->edge1(pol);
  }
  else {
    e_0 = e_1 = e;
  }
}

// @brief マネージャを返す．
inline
BddMgrImpl*
BddOp::mgr() const
{
  return mMgr;
}

END_NAMESPACE_YM_BDD

#endif // BDDOP_H
