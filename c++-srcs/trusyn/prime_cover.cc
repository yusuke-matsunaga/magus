
/// @file prime_cover.cc
/// @brief prime_cover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "trusyn.h"
#include "ym/TvFunc.h"
#include "Cube.h"


BEGIN_NAMESPACE_TRUSYN

// 単純なカバーを作る．
vector<Cube>
simplify(
  const TvFunc& func
)
{
  SizeType ni = func.input_num();

  // special case
  if ( func.count_one() == 0 ) {
    // 定数0
    return vector<Cube>{};
  }
  if ( func.count_zero() == 0 ) {
    // 定数1
    return vector<Cube>{Cube::all1(ni)}
  }

  // most binate variable を求める．
  v = most_binate_variable(func);

  func0 = func.cofactor();
  func1 = func.cofactor();

  cov0 = simplify(func0);
  cov1 = simplify(func1);

  return merge_with_containment(v, cov0, cov1);
}

// prime cover (Blake's Cannonical Form) を求める．
vector<Cube>
prime_cover(
  const TvFunc& func
)
{
}

END_NAMESPACE_TRUSYN
