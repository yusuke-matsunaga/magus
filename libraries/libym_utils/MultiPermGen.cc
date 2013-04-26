
/// @file MultiPermGen.cc
/// @brief MultiPermGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiPermGen.h"


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

// デストラクタ
MultiPermGen::~MultiPermGen()
{
}

// 最初の組み合わせを取り出す．
MultiPermGenIterator
MultiPermGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// クラス MultiPermGenIterator
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
MultiPermGenIterator::MultiPermGenIterator()
{
}

// コンストラクタ
// MultiPermGen が用いる．
MultiPermGenIterator::MultiPermGenIterator(const MultiPermGen* parent) :
  MultiGenIterator(parent)
{
}

// コピーコンストラクタ
MultiPermGenIterator::MultiPermGenIterator(const MultiPermGenIterator& src)
{
  copy(src);
}

// 代入演算子
const MultiPermGenIterator&
MultiPermGenIterator::operator=(const MultiPermGenIterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
MultiPermGenIterator
MultiPermGenIterator::operator++()
{
  for (ymuint g = group_num(); g -- > 0; ) {
    vector<int> bitmap(n(g));
    for (ymuint i = 0; i < n(g); ++ i) {
      bitmap[i] = 0;
    }
    for (ymuint i = 0; i < k(g); ++ i) {
      bitmap[elem(g)[i]] = 1;
    }
    for (ymuint i = k(g); i -- > 0; ) {
      bool found = false;
      for (ymuint j = elem(g)[i]; ++ j < n(g); ) {
	if ( bitmap[j] == 0 ) {
	  bitmap[elem(g)[i]] = 0;
	  elem(g)[i] = j;
	  bitmap[j] = 1;
	  found = true;
	  break;
	}
      }
      if ( found ) {
	ymuint pos = 0;
	for (ymuint j = i + 1; j < k(g); ++ j) {
	  for ( ; bitmap[pos] == 1; ++ pos) ;
	  bitmap[pos] = 1;
	  elem(g)[j] = pos;
	}
	break;
      }
      if ( i > 0 ) {
	bitmap[elem(g)[i]] = 0;
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

// 末尾の時に true を返す．
bool
MultiPermGenIterator::is_end() const
{
  return is_end_sub(0);
}

// grp 番目のグループが終了状態の時 true を返す．
bool
MultiPermGenIterator::is_end_sub(ymuint grp) const
{
  return elem(grp)[0] == n(grp);
}

END_NAMESPACE_YM
