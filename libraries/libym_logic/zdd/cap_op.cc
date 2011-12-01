
/// @file cap_op.cc
/// @brief cap 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

// 2つのZDDのANDを計算するapply演算
ZddEdge
ZddMgrImpl::cap_op(ZddEdge f,
		   ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  return cap_step(f, g);
}

// cap_op の下請け関数
ZddEdge
ZddMgrImpl::cap_step(ZddEdge f,
		     ZddEdge g)
{
  // 特別な場合の処理
  // 1：片方のZDDが0なら答えは0，
  // 2：両方とも1なら答えは1．
  // 3：片方が1ならもう片方を返す．
  // 4：同じZDDどうしのANDは自分自身
  // 2 は 3 でカバーされている．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return ZddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return g;
  }
  if ( g.is_one() || f == g ) {
    return f;
  }
  // この時点で f,g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if ( f > g ) {
    ZddEdge tmp = f;
    f = g;
    g = tmp;
  }

  ZddEdge result = mAndTable->get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    ZddEdge f_0, f_1;
    ZddEdge g_0, g_1;
    Var* var = split(f, g, f_0, f_1, g_0, g_1);
    ZddEdge r_0 = and_step(f_0, g_0);
    if ( r_0.is_overflow() ) {
      return ZddEdge::make_overflow();
    }
    ZddEdge r_1 = and_step(f_1, g_1);
    if ( r_1.is_overflow() ) {
      return ZddEdge::make_overflow();
    }
    result = new_node(var, r_0, r_1);
    mAndTable->put(f, g, result);
  }

  return result;
}

// 2つのZDDのdiffを計算するapply演算
ZddEdge
ZddMgrImpl::diff_op(ZddEdge f,
		    ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  // 特別な場合の処理
  // 1: 片方が0なら他方を返す．
  // 2: 片方が1なら他方の否定を返す．
  // 3: 同じZDD同士のXORは0を返す．
  // 4: 極性のみが異なる関数同士なら1を返す．
  if ( f.is_zero() ) {
    return g;
  }
  if ( g.is_zero() ) {
    return f;
  }
  if ( f.is_one() ) {
    return ~g;
  }
  if ( g.is_one() ) {
    return ~f;
  }
  if ( f == g ) {
    return ZddEdge::make_zero();
  }
  if ( check_reverse(f, g) ) {
    return ZddEdge::make_one();
  }
  // この時点で f, g は終端ではない．

  // 極性情報は落してしまう．
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  tPol ans_pol = f_pol * g_pol;
  f.normalize();
  g.normalize();

  // 対称演算なので正規化する．
  if ( f > g ) {
    ZddEdge tmp = f;
    f = g;
    g = tmp;
  }

  ZddEdge result = mXorTable->get(f, g);
  if ( result.is_error() ) {
    ZddEdge f_0, f_1;
    ZddEdge g_0, g_1;
    Var* var = split(f, g, f_0, f_1, g_0, g_1);
    ZddEdge r_0 = xor_op(f_0, g_0);
    if ( !r_0.is_invalid() ) {
      ZddEdge r_1 = xor_op(f_1, g_1);
      if ( !r_1.is_invalid() ) {
	result = new_node(var, r_0, r_1);
	mXorTable->put(f, g, result);
      }
    }
  }

  return ZddEdge(result, ans_pol);
}

// 2つの論理関数が交わっていれば kEdge1 を返す．
ZddEdge
ZddMgrImpl::check_intersect(ZddEdge f,
			       ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  // 特別な場合の処理
  // 1: 片方が0なら絶対に交わらないので 0 を返す．
  // 2: 極性のみが異なっている場合にも交わらないので 0 を返す．
  // 3: 片方が1なら交わっているので 1 を返す．
  // 4: 同じ関数なら交わっているので 1 を返す．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return ZddEdge::make_zero();
  }
  if ( f.is_one() || g.is_one() || f == g ) {
    return ZddEdge::make_one();
  }
  // この時点で f, g は終端ではない．

  // 極性は正規化されている(否定属性は0枝のみ，葉は1のみ)
  // ので f も g も正極性のときは f(1,1,...,1) = g(1,1,...,1) = 1
  // つまりすべての入力を 1 にしたときの関数値は 1 であるので
  // 交わっていることがわかる．
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  if ( f_pol == kPolPosi && g_pol == kPolPosi ) {
    return ZddEdge::make_one();
  }

  if ( f > g ) {
    ZddEdge tmp = f;
    f = g;
    g = tmp;
    tPol tmp_pol = f_pol;
    f_pol = g_pol;
    g_pol = tmp_pol;
  }

  ZddEdge result = mIntTable->get(f, g);
  if ( result.is_error() ) {
    Node* f_vp = get_node(f);
    Node* g_vp = get_node(g);
    Var* f_var = f_vp->var();
    Var* g_var = g_vp->var();
    tLevel f_level = f_var->level();
    tLevel g_level = g_var->level();
    ZddEdge f_0, f_1;
    ZddEdge g_0, g_1;
    if ( f_level < g_level ) {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_1 = g;
    }
    else if ( f_level > g_level ) {
      f_0 = f_1 = f;
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
    }
    else {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
    }
    result = check_intersect(f_0, g_0);
    if ( result.is_zero() ) {
      result = check_intersect(f_1, g_1);
    }
    mIntTable->put(f, g, result);
  }
  return result;
}

END_NAMESPACE_YM_ZDD
