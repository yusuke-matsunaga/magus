
/// @file PermGen.cc
/// @brief PermGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/PermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PermGen
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
PermGen::PermGen(ymuint n,
		 ymuint k) :
  GenBase(n, k)
{
}

// デストラクタ
PermGen::~PermGen()
{
}

// 最初の組み合わせを取り出す．
PermGenIterator
PermGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// クラス PermGenIterator
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
PermGenIterator::PermGenIterator()
{
}

// コンストラクタ
// PermGen が用いる．
PermGenIterator::PermGenIterator(const PermGen* parent) :
  GenIterator(parent)
{
}

// コピーコンストラクタ
PermGenIterator::PermGenIterator(const PermGenIterator& src)
{
  copy(src);
}

// 代入演算子
const PermGenIterator&
PermGenIterator::operator=(const PermGenIterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
PermGenIterator
PermGenIterator::operator++()
{
  vector<int> bitmap(n());
  for (ymuint i = 0; i < n(); ++ i) {
    bitmap[i] = 0;
  }
  for (ymuint i = 0; i < k(); ++ i) {
    bitmap[elem(i)] = 1;
  }
  for (ymuint i = k(); i -- > 0; ) {
    bool found = false;
    for (ymuint j = elem(i); ++ j < n(); ) {
      if ( bitmap[j] == 0 ) {
	bitmap[elem(i)] = 0;
	elem(i) = j;
	bitmap[j] = 1;
	found = true;
	break;
      }
    }
    if ( found ) {
      ymuint pos = 0;
      for (ymuint j = i + 1; j < k(); ++ j) {
	for ( ; bitmap[pos] == 1; ++ pos) ;
	bitmap[pos] = 1;
	elem(j) = pos;
      }
      break;
    }
    if ( i > 0 ) {
      bitmap[elem(i)] = 0;
    }
    else {
      elem(0) = n();
    }
  }

  return *this;
}

// 末尾の時に true を返す．
bool
PermGenIterator::is_end() const
{
  return elem(0) == n();
}

END_NAMESPACE_YM
