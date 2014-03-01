
/// @file SymOp.cc
/// @brief SymOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SymOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス SymOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
SymOp::SymOp(BddMgrImpl* mgr) :
  BddOp(mgr),
  mCompTbl1(mgr, "sym_tbl1"),
  mCompTbl2(mgr, "sym_tbl2"),
  mCompTbl3(mgr, "sym_tbl3")
{
}

// @brief デストラクタ
SymOp::~SymOp()
{
}

// @brief 演算を行う関数
// @param[in] e オペランド
// @param[in] id1, id2 対称性を調べる変数番号
// @param[in] inv 極性
// @retval true 対称だった．
// @retval false 対称でなかった．
bool
SymOp::apply(BddEdge e,
	     VarId id1,
	     VarId id2,
	     bool inv)
{
  // エラー状態のチェック
  if ( e.is_invalid() ) {
    return false;
  }

  if ( id1 == id2 ) {
    return !inv;
  }

  // 小さい方を mX，大きい方を mY とする．
  mX = mgr()->level(id1);
  mY = mgr()->level(id2);
  if ( mY < mX ) {
    ymuint32 tmp = mX;
    mX = mY;
    mY = tmp;
    VarId tmp_var = id1;
    id1 = id2;
    id2 = tmp_var;
  }
  mInv = inv;

  // 演算結果テーブル用に mX, mY を表す BDD を作る．
  BddEdge x_edge = mgr()->make_posiliteral(id1);
  mYedge = mgr()->make_posiliteral(id2);
  if ( inv ) {
    mXYedge = mgr()->and_op(x_edge, ~mYedge);
  }
  else {
    mXYedge = mgr()->and_op(x_edge, mYedge);
  }

  // 実際の演算を行なう．
  BddEdge ans = apply_step1(e);

  // 結果をブール値に直す
  return ans.is_one();
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
SymOp::sweep()
{
  mCompTbl1.sweep();
  mCompTbl2.sweep();
  mCompTbl3.sweep();
}

// @brief 変数のレベルが x よりも上の場合の関数
BddEdge
SymOp::apply_step1(BddEdge e)
{
  BddNode* node = e.get_node();
  if ( node == NULL ) {
    // e は mX と mY に無関係だった．
    return BddEdge::make_one();
  }

  ymuint level = node->level();
  if ( level > mY ) {
    // e は mX と mY に無関係だった．
    return BddEdge::make_one();
  }
  if ( level == mY ) {
    // e は mX に無関係で mY に依存している．
    return BddEdge::make_zero();
  }

  // 極性を落とす．
  e.normalize();

  BddEdge result = mCompTbl1.get(e, mXYedge);
  if ( result.is_error() ) {
    if ( level < mX ) {
      result = apply_step1(node->edge0());
      if ( result.is_one() ) {
	result = apply_step1(node->edge1());
      }
    }
    else if ( level == mX ) {
      result = apply_step2(node->edge0(), node->edge1());
    }
    else { // level > mX
      result = apply_step3(node->edge0());
      if ( result.is_one() ) {
	result = apply_step3(node->edge1());
      }
    }
    mCompTbl1.put(e, mXYedge, result);
  }
  return result;
}

// @brief 変数のレベルが x と y の間の場合の関数
BddEdge
SymOp::apply_step2(BddEdge e0,
		   BddEdge e1)
{
  // e0 は mX の edge0() の子孫
  // e1 は mY の edge1() の子孫

  if ( e0 == e1 ) {
    // このパスでは mX に無関係ということなので mY も無関係なら OK
    return apply_step3(e0);
  }

  BddNode* node0 = e0.get_node();
  BddNode* node1 = e1.get_node();
  ymuint level0 = node0 ? node0->level() : kLevelMax;
  ymuint level1 = node1 ? node1->level() : kLevelMax;
  ymuint top_level = level0;
  if ( top_level > level1 ) {
    top_level = level1;
  }

  if ( top_level > mY ) {
    // mX には依存しているが mY は依存していないのでだめ
    return BddEdge::make_zero();
  }

  BddEdge result = mCompTbl2.get(e0, e1, mXYedge);
  if ( result.is_error() ) {
    BddEdge e00;
    BddEdge e01;
    BddEdge e10;
    BddEdge e11;
    if ( level0 == top_level ) {
      bool inv0 = e0.inv();
      e00 = node0->edge0(inv0);
      e01 = node0->edge1(inv0);
    }
    else {
      e00 = e01 = e0;
    }
    if ( level1 == top_level ) {
      bool inv1 = e1.inv();
      e10 = node1->edge0(inv1);
      e11 = node1->edge1(inv1);
    }
    else {
      e10 = e11 = e1;
    }

    if ( top_level < mY ) {
      result = apply_step2(e00, e10);
      if ( result.is_one() ) {
	result = apply_step2(e01, e11);
      }
    }
    else { // top_level == mY
      if ( mInv ) {
	result = (e00 == e11) ? BddEdge::make_one() : BddEdge::make_zero();
      }
      else {
	result = (e01 == e10) ? BddEdge::make_one() : BddEdge::make_zero();
      }
    }

    mCompTbl2.put(e0, e1, mXYedge, result);
  }

  return result;
}

// @brief y のレベルのノードを含んでいないか調べる関数
BddEdge
SymOp::apply_step3(BddEdge e)
{
  if ( e.is_leaf() ) {
    // 終端ならOK
    return BddEdge::make_one();
  }

  BddNode* node = e.get_node();
  ymuint level = node->level();
  if ( level > mY ) {
    // Y を含んでいなかった．
    return BddEdge::make_one();
  }
  if ( level == mY ) {
    // Y を含んでいた，というかこれがそう．
    return BddEdge::make_zero();
  }

  // 以降は level < mY ということ．

  // 極性は関係ないのでおとしてしまう．
  e.normalize();
  BddEdge result = mCompTbl3.get(e, mYedge);
  if ( result.is_error() ) {
    // 結果が登録していなかったので子供の枝に再帰する．
    result = apply_step3(node->edge0());
    if ( result.is_one() ) {
      result = apply_step3(node->edge1());
    }
    mCompTbl3.put(e, mYedge, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
