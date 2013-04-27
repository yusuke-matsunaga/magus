
/// @file MultiCombiGen.cc
/// @brief MultiCombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiCombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiCombiGenIterator
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
MultiCombiGenIterator
MultiCombiGenIterator::operator++()
{
  for (ymuint g = group_num(); g -- > 0; ) {
    for (ymuint pos = k(g); pos -- > 0; ) {
      if ( elem(g)[pos] < n(g) - k(g) + pos ) {
	++ elem(g)[pos];
	for (ymuint pos1 = pos + 1; pos1 < k(g); ++ pos1) {
	  elem(g)[pos1] = elem(g)[pos1 - 1] + 1;
	}
	break;
      }
      else if ( pos == 0 ) {
	elem(g)[0] = n(g);
      }
    }
    if ( !is_end_sub(g) ) {
      break;
    }
    if ( g > 0 ) {
      init(g);
    }
  }

  return *this;
}

END_NAMESPACE_YM
