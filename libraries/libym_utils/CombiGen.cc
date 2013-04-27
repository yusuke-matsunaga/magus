
/// @file CombiGen.cc
/// @brief CombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/CombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CombiGenIterator
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
CombiGenIterator
CombiGenIterator::operator++()
{
  for (ymuint pos = k(); pos -- > 0; ) {
    if ( elem(pos) < n() - k() + pos ) {
      ++ elem(pos);
      for (ymuint pos1 = pos + 1; pos1 < k(); ++ pos1) {
	elem(pos1) = elem(pos1 - 1) + 1;
      }
      break;
    }
    else if ( pos == 0 ) {
      elem(0) = n();
    }
  }

  return *this;
}

END_NAMESPACE_YM
