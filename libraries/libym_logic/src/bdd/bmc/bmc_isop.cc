
/// @file bmc_isop.cc
/// @brief ISOP を生成する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// cov を積和形論理式と見なして各キューブにリテラルを追加する
Expr
sop_litand(const Expr& cov,
	   VarId varid,
	   bool inv)
{
  if ( cov.is_zero() ) {
    // 0ならそのまま
    return cov;
  }

  Expr lit = Expr::make_literal(varid, inv);
  if ( cov.is_one() ) {
    // 1なら lit を返せばよい
    return lit;
  }

  if ( cov.is_literal() || cov.is_and() ) {
    // リテラルかキューブの場合には lit を追加する．
    return cov & lit;
  }

  // あとはカバーの場合のみ
  ymuint n = cov.child_num();
  Expr ans = Expr::make_zero();
  for (ymuint i = 0; i < n; i ++) {
    Expr child = cov.child(i);
    ans |= child & lit;
  }
  return ans;
}

END_NONAMESPACE


// l と u をそれぞれ下限,上限した不完全指定論理関数
// を満たす非冗長積和形論理式を求める．
BddEdge
BddMgrClassic::isop(BddEdge l,
		    BddEdge u,
		    Expr& cover)
{
  if ( l.is_error() || u.is_error() ) {
    return BddEdge::make_error();
  }
  if ( l.is_overflow() || u.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  BddEdge ans = isop_step(l, u, cover);
  mIsopTable->clear();
  return ans;
}

// l と u をそれぞれ下限,上限した不完全指定論理関数
// のすべての prime implicants からなる prime cover を求める．
Expr
BddMgrClassic::prime_cover(BddEdge l,
			   BddEdge u)
{
  if ( l.is_invalid() || u.is_invalid() ) {
    return Expr();
  }

  Expr cover;
  (void) pc_step(l, u, cover);
  mPcTable->clear();
  return cover;
}

// [l, u] で表された不完全指定論理関数の極小サポート集合を列挙する．
// 答の関数の prime implicant が本当の極小サポート集合となっている．
BddEdge
BddMgrClassic::minimal_support(BddEdge l,
			       BddEdge u)
{
  if ( l.is_error() || u.is_error() ) {
    return BddEdge::make_error();
  }
  if ( l.is_overflow() || u.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  if ( l.is_zero() || u.is_one() ) {
    // もうオンセットとオフセットは交わらない
    return BddEdge::make_one();
  }
  if ( l.is_one() || u.is_zero() ) {
    // オンセットとオフセットが交わっている
    return BddEdge::make_zero();
  }

  BddEdge result = mMinsupTable->get(l, u);
  if ( result.is_error() ) {
    BddEdge l0, l1;
    BddEdge u0, u1;
    ymuint level = split(l, u, l0, l1, u0, u1);

    // level を含む場合の極小サポート
    BddEdge r_dep = minimal_support(l0, u0);
    if ( !r_dep.is_zero() ) {
      BddEdge r_dep1 = minimal_support(l1, u1);
      r_dep = and_op(r_dep, r_dep1);
    }

    // level を含まない場合の極小サポート
    BddEdge r_indep = minimal_support(or_op(l0, l1), and_op(u0, u1));

    result = new_node(level, r_indep, r_dep);
    mMinsupTable->put(l, u, result);
  }
  return result;
}

// isop 用の再帰関数
BddEdge
BddMgrClassic::isop_step(BddEdge l,
			 BddEdge u,
			 Expr& cov)
{
  if ( l.is_zero() ) {
    // 0 がもっとも簡単な答え
    cov = Expr::make_zero();
    return BddEdge::make_zero();
  }

  if ( u.is_one() ) {
    // 1 がもっとも簡単な答え
    cov = Expr::make_one();
    return BddEdge::make_one();
  }

  // 演算結果テーブルを探す．
  BddEdge result = mIsopTable->get(l, u, cov);
  if ( result.is_error() ) {
    // 見つからなかった．
    BddEdge l_0, l_1;
    BddEdge u_0, u_1;
    ymuint level = split(l, u, l_0, l_1, u_0, u_1);
    BddEdge var_edge = new_node(level, BddEdge::make_zero(), BddEdge::make_one());
    VarId var = varid(level);
    Expr p_0;
    BddEdge z_0 = and_op(l_0, ~u_1);
    BddEdge c_0;
    BddEdge cc_0;
    if ( !z_0.is_invalid() ) {
      c_0 = isop_step(z_0, u_0, p_0);
      if ( !c_0.is_invalid() ) {
	cc_0 = and_op(c_0, ~var_edge);
	if ( !cc_0.is_invalid() ) {
	  p_0 = sop_litand(p_0, var, true);
	}
      }
    }
    Expr p_1;
    BddEdge z_1 = and_op(l_1, ~u_0);
    BddEdge c_1;
    BddEdge cc_1;
    if ( !z_1.is_invalid() ) {
      c_1 = isop_step(z_1, u_1, p_1);
      if ( !c_1.is_invalid() ) {
	cc_1 = and_op(c_1, var_edge);
	if ( !cc_1.is_invalid() ) {
	  p_1 = sop_litand(p_1, var, false);
	}
      }
    }

    Expr p_2;
    BddEdge h_01 = and_op(l_0, ~c_0);
    if ( !h_01.is_invalid() ) {
      BddEdge h_02 = and_op(l_1, ~c_1);
      if ( !h_02.is_invalid() ) {
	BddEdge h_0  = or_op(h_01, h_02);
	if ( !h_0.is_invalid() ) {
	  BddEdge h_1  = and_op(u_0, u_1);
	  if ( !h_1.is_invalid() ) {
	    BddEdge r_0 = isop_step(h_0, h_1, p_2);
	    if ( !r_0.is_invalid() ) {
	      BddEdge tmp = or_op(cc_0, cc_1);
	      result = or_op(tmp, r_0);
	      cov = p_0 | p_1 | p_2;
	      mIsopTable->put(l, u, result, cov);
	    }
	  }
	}
      }
    }
  }

  return result;
}

// prime_cover 用の再帰関数
BddEdge
BddMgrClassic::pc_step(BddEdge l,
		       BddEdge u,
		       Expr& cov)
{
  if ( l.is_zero() ) {
    // 0 がもっとも簡単な答え
    cov = Expr::make_zero();
    return BddEdge::make_zero();
  }

  if ( u.is_one() ) {
    // 1 がもっとも簡単な答え
    cov = Expr::make_one();
    return BddEdge::make_one();
  }

  // 演算結果テーブルを探す．
  BddEdge result = mPcTable->get(l, u, cov);
  if ( result.is_error() ) {
    // 見つからなかった．
    BddEdge l_0, l_1;
    BddEdge u_0, u_1;
    ymuint level = split(l, u, l_0, l_1, u_0, u_1);
    BddEdge var_edge = new_node(level, BddEdge::make_zero(), BddEdge::make_one());
    VarId var(level);

    // \bar{x} を含む prime implicants の生成
    Expr p_0;
    BddEdge z_0 = and_op(l_0, ~u_1);
    BddEdge c_0;
    BddEdge cc_0;
    if ( !z_0.is_invalid() ) {
      c_0 = pc_step(z_0, u_0, p_0);
      if ( !c_0.is_invalid() ) {
	cc_0 = and_op(c_0, ~var_edge);
	if ( !cc_0.is_invalid() ) {
	  p_0 = sop_litand(p_0, var, true);
	}
      }
    }

    // x を含む prime implicatns の生成
    Expr p_1;
    BddEdge z_1 = and_op(l_1, ~u_0);
    BddEdge c_1;
    BddEdge cc_1;
    if ( !z_1.is_invalid() ) {
      c_1 = pc_step(z_1, u_1, p_1);
      if ( !c_1.is_invalid() ) {
	cc_1 = and_op(c_1, var_edge);
      }
    }
    p_1 = sop_litand(p_1, var, false);

    // x/\bar{x} を含まない prime implicants の生成
    Expr p_2;
    BddEdge h_01 = and_op(l_0, ~c_0);
    if ( !h_01.is_invalid() ) {
      BddEdge h_02 = and_op(l_1, ~c_1);
      if ( !h_02.is_invalid() ) {
	BddEdge h_0  = or_op(h_01, h_02);
	if ( !h_0.is_invalid() ) {
	  BddEdge h_1  = and_op(u_0, u_1);
	  if ( !h_1.is_invalid() ) {
	    BddEdge r_0 = pc_step(h_0, h_1, p_2);
	    if ( !r_0.is_invalid() ) {
	      BddEdge tmp = or_op(cc_0, cc_1);
	      result = or_op(tmp, r_0);
	      cov = p_0 | p_1 | p_2;
	      mPcTable->put(l, u, result, cov);
	    }
	  }
	}
      }
    }
  }

  return result;
}

END_NAMESPACE_YM_BDD
