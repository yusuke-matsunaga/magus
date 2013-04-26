
/// @file MultiCombiGen.cc
/// @brief MultiCombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiCombiGen.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス MutlCombiGen
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiCombiGen::MultiCombiGen(const vector<pair<ymuint, ymuint> >& nk_array) :
  MultiGenBase(nk_array)
{
}

// デストラクタ
MultiCombiGen::~MultiCombiGen()
{
}

// 最初の組み合わせを取り出す．
MultiCombiGenIterator
MultiCombiGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// クラス MultiCombiGenIterator
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
MultiCombiGenIterator::MultiCombiGenIterator()
{
}

// コンストラクタ
// MultiCombiGen が用いる．
MultiCombiGenIterator::MultiCombiGenIterator(const MultiCombiGen* parent) :
  MultiGenIterator(parent)
{
}

// コピーコンストラクタ
MultiCombiGenIterator::MultiCombiGenIterator(const MultiCombiGenIterator& src)
{
  copy(src);
}

// 代入演算子
const MultiCombiGenIterator&
MultiCombiGenIterator::operator=(const MultiCombiGenIterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
MultiCombiGenIterator
MultiCombiGenIterator::operator++()
{
  for (ymuint g = group_num(); g -- > 0; ) {
    if ( !is_end_sub(g) ) {
      next(g, k(g) - 1);
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
MultiCombiGenIterator::is_end() const
{
  return is_end_sub(0);
}

// operator++() のサブルーティン
void
MultiCombiGenIterator::next(ymuint g,
			    ymuint pos)
{
  ++ elem(g)[pos];
  if ( elem(g)[pos] == n(g) - k(g) + pos + 1) {
    if ( pos > 0 ) {
      next(g, pos - 1);
      elem(g)[pos] = elem(g)[pos - 1] + 1;
    }
  }
}

// grp 番目のグループが終了状態の時 true を返す．
bool
MultiCombiGenIterator::is_end_sub(ymuint grp) const
{
  return elem(grp)[k(grp) - 1] == n(grp);
}

END_NAMESPACE_YM
