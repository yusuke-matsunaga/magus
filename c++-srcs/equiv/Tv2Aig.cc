
/// @file Tv2Aig.cc
/// @brief Tv2Aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Tv2Aig.h"


BEGIN_NAMESPACE_FRAIG

// @brief 真理値表に対応するノード(木)を作る．
FraigHandle
FraigMgr::make_tv(
  const TvFunc& func,
  const vector<FraigHandle>& inputs
)
{
  Tv2Aig op{*this, inputs};
  auto ans = op.conv(func, 0);
  return ans;
}

// @brief TvFunc を AIG に変換する．
FraigHandle
Tv2Aig::conv(
  const TvFunc& func,
  SizeType pos
)
{
  if ( func.is_zero() ) {
    return mMgr.make_zero();
  }
  if ( func.is_one() ) {
    return mMgr.make_one();
  }

  if ( mAigMap.count(func) > 0 ) {
    return mAigMap.at(func);
  }

  for ( ; pos < mInputs.size(); ++ pos ) {
    VarId var{pos};
    auto f0 = func.cofactor(var, true);
    auto f1 = func.cofactor(var, false);
    if ( f0 != f1 ) {
      auto r0 = conv(f0, pos + 1);
      auto r1 = conv(f1, pos + 1);
      return mMgr.make_mux(mInputs[pos], r0, r1);
    }
  }
  ASSERT_NOT_REACHED;
  return mMgr.make_zero(); // ダミー
}

END_NAMESPACE_FRAIG
