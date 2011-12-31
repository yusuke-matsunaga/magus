
/// @file bmc_symmetry.cc
/// @brief 対称性の判定を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

ymuint x_level;
ymuint y_level;

BddEdge xy_edge;
BddEdge y_edge;

END_NONAMESPACE

// xを含まないパスeがyも含まなければBddEdge::make_one()(Trueのつもり)を返す．
BddEdge
BddMgrClassic::cs_step2(BddEdge e)
{
  BddNode* vp = e.get_node();
  if ( vp == 0 ) {
    return BddEdge::make_one();
  }
  ymuint level = vp->level();
  if ( level > y_level ) {
    return BddEdge::make_one();
  }
  else if ( level == y_level ) {
    return BddEdge::make_zero();
  }

  e.normalize();
  BddEdge result = mCs2Table->get(e, y_edge);
  if ( result.is_error() ) {
    result = cs_step2(vp->edge0());
    if ( result == BddEdge::make_one() ) {
      result = cs_step2(vp->edge1());
    }
    mCs2Table->put(e, y_edge, result);
  }
  return result;
}

// xを0にしたパスe1と1にしたパスe2をくらべて，それぞれyを1/0にした結果が
// 同一ならばBddEdge::make_one()(True)を返す．つまりこのパスに関してはxとyは対称
BddEdge
BddMgrClassic::cs_step1(BddEdge e1,
			BddEdge e2,
			tPol sympol)
{
  if ( e1 == e2 ) {
    return cs_step2(e1);
  }

  BddNode* vp1 = e1.get_node();
  BddNode* vp2 = e2.get_node();
  ymuint level1 = vp1 ? vp1->level() : kLevelMax;
  ymuint level2 = vp2 ? vp2->level() : kLevelMax;
  ymuint top_level = level1;
  if ( top_level > level2 ) {
    top_level = level2;
  }
  if ( top_level > y_level ) {
    // x に依存していたが y に依存していないので false
    return BddEdge::make_zero();
  }

  BddEdge result = mCs1Table->get(e1, e2, xy_edge);
  if ( result.is_error() ) {
    BddEdge e10;
    BddEdge e11;
    BddEdge e20;
    BddEdge e21;
    if ( level1 == top_level ) {
      tPol pol1 = e1.pol();
      e10 = vp1->edge0(pol1);
      e11 = vp1->edge1(pol1);
    }
    else {
      e10 = e11 = e1;
    }
    if ( level2 == top_level ) {
      tPol pol2 = e2.pol();
      e20 = vp2->edge0(pol2);
      e21 = vp2->edge1(pol2);
    }
    else {
      e20 = e21 = e2;
    }
    if ( top_level < y_level ) {
      result = cs_step1(e10, e20, sympol);
      if ( result == BddEdge::make_one() ) {
	result = cs_step1(e11, e21, sympol);
      }
    }
    else { // top_level == y_level
      if ( sympol == kPolPosi ) {
	result = (e11 == e20) ? BddEdge::make_one() : BddEdge::make_zero();
      }
      else {
	result = (e10 == e21) ? BddEdge::make_one() : BddEdge::make_zero();
      }
    }
    mCs1Table->put(e1, e2, xy_edge, result);
  }
  return result;
}

BddEdge
BddMgrClassic::cs_step(BddEdge e,
		       tPol sympol)
{
  BddNode* vp = e.get_node();
  if ( vp == 0 ) {
    // x も y も無関係なら true を返す．
    return BddEdge::make_one();
  }

  ymuint level = vp->level();
  if ( level > y_level ) {
    // x も y も無関係なら true を返す．
    return BddEdge::make_one();
  }
  if ( level == y_level ) {
    // ここまでのパスにxが含まれず，yが含まれているので false
    return BddEdge::make_zero();
  }

  // 極性は落としてしまう．
  e.normalize();
  BddEdge result = mCsTable->get(e, xy_edge);
  if ( result.is_error() ) {
    if ( level < x_level ) {
      result = cs_step(vp->edge0(), sympol);
      if ( result == BddEdge::make_one() ) {
	result = cs_step(vp->edge1(), sympol);
      }
    }
    else if ( level == x_level ) {
      result = cs_step1(vp->edge0(), vp->edge1(), sympol);
    }
    else { // level > x_level
      result = cs_step2(vp->edge0());
      if ( result == BddEdge::make_one() ) {
	result = cs_step2(vp->edge1());
      }
    }
    mCsTable->put(e, xy_edge, result);
  }
  return result;
}

// 関数 f に対して変数 x, y が互いに対称かを調べる．
// 結果はブール値であり新たなノードを作るわけではないが，
// 演算結果テーブルが拡張される可能性があるのでメモリ不足によって
// エラーとなる可能性がある．
bool
BddMgrClassic::check_symmetry(BddEdge e,
			      VarId x,
			      VarId y,
			      tPol pol)
{
  if ( x == y ) {
    // これは前もってチェックしておくべき
    return false;
  }

  x_level = level(x);
  y_level = level(y);
  if ( y_level < x_level ) {
    ymuint tmp = x_level;
    x_level = y_level;
    y_level = tmp;
  }

  BddEdge x_edge = make_posiliteral(x);
  y_edge = make_posiliteral(y);
  if ( pol == kPolPosi ) {
    xy_edge = and_op(x_edge, y_edge);
  }
  else {
    xy_edge = and_op(x_edge, ~y_edge);
  }

  BddEdge ans = cs_step(e, pol);

  return ans == BddEdge::make_one();
}

END_NAMESPACE_YM_BDD
