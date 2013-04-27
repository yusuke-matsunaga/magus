
/// @file MultiPermGen.cc
/// @brief MultiPermGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiPermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiPermGenIterator
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
MultiPermGenIterator
MultiPermGenIterator::operator++()
{
  for (ymuint g = group_num(); g -- > 0; ) {
    vector<int> bitmap(n(g), 0);
    for (ymuint pos = 0; pos < k(g); ++ pos) {
      bitmap[elem(g)[pos]] = 1;
    }
    for (ymuint pos = k(g); pos -- > 0; ) {
      bool found = false;
      for (ymuint val = elem(g)[pos]; ++ val < n(g); ) {
	if ( bitmap[val] == 0 ) {
	  bitmap[elem(g)[pos]] = 0;
	  elem(g)[pos] = val;
	  bitmap[val] = 1;
	  found = true;
	  break;
	}
      }
      if ( found ) {
	ymuint val = 0;
	for (ymuint j = pos + 1; j < k(g); ++ j) {
	  for ( ; bitmap[val] == 1; ++ val) ;
	  bitmap[val] = 1;
	  elem(g)[j] = val;
	  ++ val;
	}
	break;
      }
      if ( pos > 0 ) {
	bitmap[elem(g)[pos]] = 0;
      }
      else {
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
