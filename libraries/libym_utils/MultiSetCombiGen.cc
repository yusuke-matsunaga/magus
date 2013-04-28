
/// @file MultiSetCombiGen.cc
/// @brief MultiSetCombiGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetCombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiSetCombiGen
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を求める．
void
MultiSetCombiGen::operator++()
{
  ymuint ng = group_num();
  vector<ymuint> count(ng, 0);
  for (ymuint i = 0; i < k(); ++ i) {
    ymuint v = elem(i);
    ++ count[v];
  }

  // 唯一，後置型の演算子の意味のある使い方
  // 符号なし数なので pos >= 0 は常に成り立ってしまう．
  // あえて pos <= k という使い方もできないことはないが，，，，
  for (ymuint pos = k(); pos -- > 0; ) {
    bool found = false;
    for (ymuint v = elem(pos); ++ v < ng; ++ v) {
      if ( count[v] < n(v) ) {
	-- count[elem(pos)];
	elem(pos) = v;
	++ count[v];
	found = true;
	break;
      }
    }
    if ( found ) {
      ymuint g = elem(pos);
      for (ymuint pos1 = pos + 1; pos1 < k(); ++ pos1) {
	for ( ; g < ng; ++ g) {
	  if ( count[g] < n(g) ) {
	    break;
	  }
	}
	if ( g == ng ) {
	  elem(0) = ng;
	  return;
	}
	++ count[g];
	elem(pos1) = g;
      }
      break;
    }
    if ( pos > 0 ) {
      -- count[elem(pos)];
    }
    else {
      elem(0) = ng;
    }
  }
}

END_NAMESPACE_YM
