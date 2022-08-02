
/// @file Expr2Aig.cc
/// @brief Expr2Aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Expr2Aig.h"


BEGIN_NAMESPACE_FRAIG

// @brief 論理式に対応するノード(木)をつくる．
FraigHandle
FraigMgr::make_expr(
  const Expr& expr,
  const vector<FraigHandle>& inputs
)
{
  Expr2Aig op{*this, inputs};
  return op.conv(expr);
}

// @brief 論理式に対応するノード(木)をつくる．
FraigHandle
Expr2Aig::conv(
  const Expr& expr
)
{
  if ( expr.is_zero() ) {
    return mMgr.make_zero();
  }
  if ( expr.is_one() ) {
    return mMgr.make_one();
  }
  if ( expr.is_posi_literal() ) {
    VarId var = expr.varid();
    int id = var.val();
    ASSERT_COND( id < mInputs.size() );
    return mInputs[id];
  }
  if ( expr.is_nega_literal() ) {
    VarId var = expr.varid();
    int id = var.val();
    ASSERT_COND( id < mInputs.size() );
    return ~mInputs[id];
  }

  SizeType n = expr.child_num();
  vector<FraigHandle> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    edge_list[i] = conv(expr.child(i));
  }
  if ( expr.is_and() ) {
    return mMgr.make_and(edge_list);
  }
  if ( expr.is_or() ) {
    return mMgr.make_or(edge_list);
  }
  if ( expr.is_xor() ) {
    return mMgr.make_xor(edge_list);
  }

  ASSERT_NOT_REACHED;
  return mMgr.make_zero();
}

END_NAMESPACE_FRAIG
