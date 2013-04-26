
/// @file CombiGen.cc
/// @brief CombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/CombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CombiGen
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
CombiGen::CombiGen(ymuint n,
		   ymuint k) :
  GenBase(n, k)
{
}

// デストラクタ
CombiGen::~CombiGen()
{
}

// 最初の組み合わせを取り出す．
CombiGenIterator
CombiGen::begin()
{
  return iterator(this);
}


//////////////////////////////////////////////////////////////////////
// クラス CombiGenIterator
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
CombiGenIterator::CombiGenIterator()
{
}

// コンストラクタ
// CombiGen が用いる．
CombiGenIterator::CombiGenIterator(const CombiGen* parent) :
  GenIterator(parent)
{
}

// コピーコンストラクタ
CombiGenIterator::CombiGenIterator(const CombiGenIterator& src)
{
  copy(src);
}

// 代入演算子
const CombiGenIterator&
CombiGenIterator::operator=(const CombiGenIterator& src)
{
  copy(src);
  return *this;
}

// 次の要素を求める．
CombiGenIterator
CombiGenIterator::operator++()
{
  if ( elem(k() - 1) < n() ) {
    next(k() - 1);
  }
  return *this;
}

// 末尾の時に true を返す．
bool
CombiGenIterator::is_end() const
{
  // 末尾の時には範囲外の値(= n())を持っている．
  return elem(k() - 1) == n();
}

// operator++() のサブルーティン
void
CombiGenIterator::next(ymuint pos)
{
  ++ elem(pos);
  if ( elem(pos) == n() - k() + pos + 1) {
    if ( pos > 0 ) {
      next(pos - 1);
      elem(pos) = elem(pos - 1) + 1;
    }
  }
}

END_NAMESPACE_YM
