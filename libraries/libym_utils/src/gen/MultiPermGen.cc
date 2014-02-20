
/// @file MultiPermGen.cc
/// @brief MultiPermGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/MultiPermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiPermGen
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiPermGen::MultiPermGen(const vector<pair<ymuint, ymuint> >& nk_array) :
  MultiGenBase(nk_array)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MultiPermGen::MultiPermGen(const MultiPermGen& src) :
  MultiGenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const MultiPermGen&
MultiPermGen::operator=(const MultiPermGen& src)
{
  copy(src);
  return *this;
}

// デストラクタ
MultiPermGen::~MultiPermGen()
{
}

// 次の要素を求める．
void
MultiPermGen::operator++()
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
      init_group(g);
    }
  }
}

END_NAMESPACE_YM
