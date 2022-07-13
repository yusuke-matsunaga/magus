
/// @file trusyn.cc
/// @brief trusyn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "trusyn.h"
#include "Aig.h"


BEGIN_NAMESPACE_TRUSYN

Aig
trusyn(
  const vector<TvFunc>& f_vect
)
{
  SizeType O = f_vect.size();
  ASSERT_COND( O > 0 );
  SizeType I = f_vect[0].input_num();
  for ( SizeType i = 1; i < O; ++ i ) {
    ASSERT_COND( f_vect[i].input_num() == I );
  }

}

END_NAMESPACE_TRUSYN
