
/// @file MultiCombiGen.cc
/// @brief MultiCombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MultiCombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiCombiGen
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiCombiGen::MultiCombiGen(const vector<pair<ymuint, ymuint> >& nk_array) :
  MultiGenBase(nk_array)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MultiCombiGen::MultiCombiGen(const MultiCombiGen& src) :
  MultiGenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const MultiCombiGen&
MultiCombiGen::operator=(const MultiCombiGen& src)
{
  copy(src);
  return *this;
}

// デストラクタ
MultiCombiGen::~MultiCombiGen()
{
}

// 次の要素を求める．
void
MultiCombiGen::operator++()
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
      init_group(g);
    }
  }
}

END_NAMESPACE_YM
