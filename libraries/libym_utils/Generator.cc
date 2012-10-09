
/// @file Generator.cc
/// @brief 組み合わせ生成器と順列生成器の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Generator.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
GenBase::GenBase(ymuint n,
		 ymuint k) :
  mN(n),
  mK(k)
{
}

// デストラクタ
GenBase::~GenBase()
{
}


//////////////////////////////////////////////////////////////////////
// クラス GenIterator
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
GenIterator::GenIterator() :
  mElem(0),
  mParent(NULL)
{
}

// コンストラクタ
// 継承クラスが用いる．
GenIterator::GenIterator(const GenBase* parent) :
  mElem(parent->k()),
  mParent(parent)
{
  for (ymuint i = 0; i < mParent->k(); ++ i) {
    mElem[i] = i;
  }
}

// @brief デストラクタ
GenIterator::~GenIterator()
{
}

// 内容をコピーする関数
void
GenIterator::copy(const GenIterator& src)
{
  mElem = src.mElem;
  mParent = src.mParent;
}


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


//////////////////////////////////////////////////////////////////////
// クラス MultiGenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiGenBase::MultiGenBase(const vector<pair<ymuint, ymuint> >& nk_array) :
  mNkArray(nk_array)
{
}

// デストラクタ
MultiGenBase::~MultiGenBase()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MultiGenIterator
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
MultiGenIterator::MultiGenIterator() :
  mElemArray(0),
  mParent(NULL)
{
}

// コンストラクタ
// 継承クラスが用いる．
MultiGenIterator::MultiGenIterator(const MultiGenBase* parent) :
  mElemArray(parent->group_num()),
  mParent(parent)
{
  ymuint group_num = parent->group_num();
  for (ymuint g = 0; g < group_num; ++ g) {
    mElemArray[g] = new vector<ymuint>(k(g));
    init(g);
  }
}

// コピーする．
void
MultiGenIterator::copy(const MultiGenIterator& src)
{
  if ( this != &src ) {
    free();
    mParent = src.mParent;
    if ( mParent ) {
      ymuint group_num = mParent->group_num();
      mElemArray.resize(group_num);
      for (ymuint g = 0; g < group_num; ++ g) {
	mElemArray[g] = new vector<ymuint>(k(g));
	elem(g) = src.elem(g);
      }
    }
    else {
      mElemArray.resize(0);
    }
  }
}

// デストラクタ
MultiGenIterator::~MultiGenIterator()
{
  free();
}

// grp 番目のグループの要素配列を初期化する．
void
MultiGenIterator::init(ymuint grp)
{
  for (ymuint i = 0; i < k(grp); ++ i) {
    elem(grp)[i] = i;
  }
}

// grp 番目のグループの要素配列を得る．
vector<ymuint>&
MultiGenIterator::elem(ymuint g)
{
  assert_cond(mElemArray[g], __FILE__, __LINE__);
  return *mElemArray[g];
}

// grp 番目のグループの要素配列を得る．
// こちらは const 版
const vector<ymuint>&
MultiGenIterator::elem(ymuint g) const
{
  return *mElemArray[g];
}

// 確保したメモリを解放する
void
MultiGenIterator::free()
{
  ymuint n = mElemArray.size();
  for (ymuint g = 0; g < n; ++ g) {
    delete mElemArray[g];
    mElemArray[g] = NULL;
  }
}


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
