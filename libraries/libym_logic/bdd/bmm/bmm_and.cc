
/// @file bmm_and.cc
/// @brief AND 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrModern.h"
#include "BmmCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

// 2つのBDDのANDを計算するapply演算
// これで，AND,NAND,OR,NORをすべて計算する．
BddEdge
BddMgrModern::and_op(BddEdge f,
		     BddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  return and_step(f, g);
}

// and_op() の下請け関数
BddEdge
BddMgrModern::and_step(BddEdge f,
		       BddEdge g)
{
  // 特別な場合の処理
  // 1：片方のBDDが0なら答えは0，互いに否定の関係でも答えは0．
  // 2：両方とも1なら答えは1．
  // 3：片方が1ならもう片方を返す．
  // 4：同じBDDどうしのANDは自分自身
  // 2 は 3 でカバーされている．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return g;
  }
  if ( g.is_one() || f == g ) {
    return f;
  }
  // この時点で f,g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if (f > g) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  BddEdge result = mAndTable->get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    Var* var = split(f, g, f_0, f_1, g_0, g_1);
    BddEdge r_0 = and_step(f_0, g_0);
    BddEdge r_1 = and_step(f_1, g_1);
    result = new_node(var, r_0, r_1);
    mAndTable->put(f, g, result);
  }

  return result;
}

// 2つのBDDのXORを計算するapply演算
BddEdge
BddMgrModern::xor_op(BddEdge f,
		     BddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  return xor_step(f, g);
}

// xor_op() の下請け関数
BddEdge
BddMgrModern::xor_step(BddEdge f,
		       BddEdge g)
{
  // 特別な場合の処理
  // 1: 片方が0なら他方を返す．
  // 2: 片方が1なら他方の否定を返す．
  // 3: 同じBDD同士のXORは0を返す．
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
    return BddEdge::make_zero();
  }
  if ( check_reverse(f, g) ) {
    return BddEdge::make_one();
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
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  BddEdge result = mXorTable->get(f, g);
  if ( result.is_error() ) {
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    Var* var = split(f, g, f_0, f_1, g_0, g_1);
    BddEdge r_0 = xor_step(f_0, g_0);
    BddEdge r_1 = xor_step(f_1, g_1);
    result = new_node(var, r_0, r_1);
    mXorTable->put(f, g, result);
  }

  return BddEdge(result, ans_pol);
}

// 2つの論理関数が交わっていれば BddEdge::make_one() を返す．
BddEdge
BddMgrModern::check_intersect(BddEdge f,
			      BddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  // 特別な場合の処理
  // 1: 片方が0なら絶対に交わらないので 0 を返す．
  // 2: 極性のみが異なっている場合にも交わらないので 0 を返す．
  // 3: 片方が1なら交わっているので 1 を返す．
  // 4: 同じ関数なら交わっているので 1 を返す．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() || g.is_one() || f == g ) {
    return BddEdge::make_one();
  }
  // この時点で f, g は終端ではない．

  // 極性は正規化されている(否定属性は0枝のみ，葉は1のみ)
  // ので f も g も正極性のときは f(1,1,...,1) = g(1,1,...,1) = 1
  // つまりすべての入力を 1 にしたときの関数値は 1 であるので
  // 交わっていることがわかる．
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  if ( f_pol == kPolPosi && g_pol == kPolPosi ) {
    return BddEdge::make_one();
  }

  if ( f > g ) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
    tPol tmp_pol = f_pol;
    f_pol = g_pol;
    g_pol = tmp_pol;
  }

  BddEdge result = mIntTable->get(f, g);
  if ( result.is_error() ) {
    Node* f_vp = get_node(f);
    Node* g_vp = get_node(g);
    Var* f_var = f_vp->var();
    Var* g_var = g_vp->var();
    ymuint f_level = f_var->level();
    ymuint g_level = g_var->level();
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
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
    if ( result == BddEdge::make_zero() ) {
      result = check_intersect(f_1, g_1);
    }
    mIntTable->put(f, g, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
