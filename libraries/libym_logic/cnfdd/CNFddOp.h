#ifndef CNFDDOP_H
#define CNFDDOP_H

/// @file CNFddOp.h
/// @brief CNFddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "CNFddMgrImpl.h"
#include "CNFddEdge.h"
#include "CNFddNode.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class CNFddOp CNFddOp.h "CNFddOp.h"
/// @brief CNFDD の演算を行うクラス
//////////////////////////////////////////////////////////////////////
class CNFddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  CNFddOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~CNFddOp() { }


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

  /// @brief ノードを生成する．
  /// @param[in] level レベル
  /// @param[in] e_0 0枝
  /// @param[in] e_p p枝
  /// @param[in] e_n n枝
  CNFddEdge
  new_node(ymuint level,
	   CNFddEdge e_0,
	   CNFddEdge e_p,
	   CNFddEdge e_n);

  /// @brief split() の下請け関数
  /// @param[in] top_level 根のレベル
  /// @param[in] level レベル
  /// @param[in] e 枝
  /// @param[in] node ノード
  /// @param[out] e_0 0枝
  /// @param[out] e_p p枝
  /// @param[out] e_n n枝
  static
  void
  split1(ymuint top_level,
	 ymuint level,
	 CNFddEdge e,
	 CNFddNode* node,
	 CNFddEdge& e_0,
	 CNFddEdge& e_p,
	 CNFddEdge& e_n);

  /// @brief マネージャを返す．
  CNFddMgrImpl&
  mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  CNFddMgrImpl& mMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
// @param[in] mgr マネージャ
inline
CNFddOp::CNFddOp(CNFddMgrImpl& mgr) :
  mMgr(mgr)
{
  mMgr.mOpList.push_back(this);
}

// @param[in] level レベル
// @param[in] e_0 0枝
// @param[in] e_p p枝
// @param[in] e_n n枝
inline
CNFddEdge
CNFddOp::new_node(ymuint level,
		  CNFddEdge e_0,
		  CNFddEdge e_p,
		  CNFddEdge e_n)
{
  return mMgr.new_node(level, e_0, e_p, e_n);
}

// @brief split() の下請け関数
// @param[in] top_level 根のレベル
// @param[in] level レベル
// @param[in] e 枝
// @param[in] node ノード
// @param[out] e_0 0枝
// @param[out] e_p p枝
// @param[out] e_n n枝
inline
void
CNFddOp::split1(ymuint top_level,
		ymuint level,
		CNFddEdge e,
		CNFddNode* node,
		CNFddEdge& e_0,
		CNFddEdge& e_p,
		CNFddEdge& e_n)
{
  if ( top_level == level ) {
    e_0 = node->edge_0();
    e_p = node->edge_p();
    e_n = node->edge_n();
    e_0.add_zattr(e.zattr());
  }
  else {
    e_0 = e;
    e_p = e_n = CNFddEdge::make_zero();
  }
}

// @brief マネージャを返す．
inline
CNFddMgrImpl&
CNFddOp::mgr() const
{
  return mMgr;
}

END_NAMESPACE_YM_CNFDD

#endif // CNFDDOP_H
