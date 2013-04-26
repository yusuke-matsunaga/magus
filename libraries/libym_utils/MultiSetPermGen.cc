
/// @file MultiSetPermGen.cc
/// @brief MultiSetPermGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetPermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MspgIterator
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
MspgIterator::MspgIterator()
{
  mParent = NULL;
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MspgIterator::MspgIterator(const MspgIterator& src) :
  mElem(src.mElem),
  mParent(src.mParent)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const MspgIterator&
MspgIterator::operator=(const MspgIterator& src)
{
  if ( this != &src ) {
    mElem = src.mElem;
    mParent = src.mParent;
  }
  return *this;
}

// @brief デストラクタ
MspgIterator::~MspgIterator()
{
}

// @brief 要素の取得
// @param[in] pos 取り出す要素の位置
ymuint
MspgIterator::operator()(ymuint pos) const
{
  return mElem[pos];
}

// @brief 次の要素を求める．
// @return 次の要素を指す反復子
MspgIterator
MspgIterator::operator++()
{
  if ( mParent == NULL ) {
    return MspgIterator();
  }

  ymuint n = mParent->group_num();
  ymuint k = mParent->k();
  vector<ymuint> count(n, 0);
  for (ymuint i = 0; i < k; ++ i) {
    ymuint v = mElem[i];
    ++ count[v];
  }

  // 唯一，後置型の演算子の意味のある使い方
  // 符号なし数なので pos >= 0 は常に成り立ってしまう．
  // あえて pos <= k という使い方もできないことはないが，，，，
  for (ymuint pos = k; pos -- > 0; ) {
    bool found = false;
    for (ymuint v = mElem[pos]; ++ v < n; ++ v) {
      if ( count[v] < mParent->n(v) ) {
	-- count[mElem[pos]];
	mElem[pos] = v;
	++ count[v];
	found = true;
	break;
      }
    }
    if ( found ) {
      ymuint g = 0;
      for (ymuint pos1 = pos + 1; pos1 < k; ++ pos1) {
	for ( ; count[g] == mParent->n(g); ++ g) ;
	++ count[g];
	mElem[pos1] = g;
      }
      break;
    }
    if ( pos > 0 ) {
      -- count[mElem[pos]];
    }
    else {
      mElem[0] = n;
    }
  }

  return *this;
}

// @brief 末尾のチェック
// @return 末尾の時に true を返す．
bool
MspgIterator::is_end() const
{
  if ( mParent == NULL ) {
    return true;
  }
  return mElem[0] == mParent->group_num();
}

// @brief コンストラクタ
// @param[in] parent 親の MultiSetPermGen オブジェクト
MspgIterator::MspgIterator(const MultiSetPermGen* parent)
{
  assert_cond( parent != NULL, __FILE__, __LINE__);

  mParent = parent;
  ymuint k = parent->k();
  mElem.resize(k);
  ymuint pos = 0;
  ymuint count = 0;
  for (ymuint i = 0; i < k; ++ i) {
    if ( count >= parent->n(pos) ) {
      ++ pos;
      count = 0;
    }
    assert_cond( count < parent->n(pos), __FILE__, __LINE__);
    mElem[i] = pos;
    ++ count;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MultiSetPermGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
MultiSetPermGen::MultiSetPermGen(const vector<ymuint>& num_array,
				 ymuint k) :
  mNumArray(num_array),
  mK(k)
{
}

// @brief デストラクタ
MultiSetPermGen::~MultiSetPermGen()
{
}

// @brief 要素の種類の数を得る．
ymuint
MultiSetPermGen::group_num() const
{
  return mNumArray.size();
}

// @brief 各要素の重複度を得る．
// @param[in] grp ( 0 <= grp < group_num() )
ymuint
MultiSetPermGen::n(ymuint grp) const
{
  assert_cond( grp < group_num(), __FILE__, __LINE__);
  return mNumArray[grp];
}

// @brief 選択する要素数を返す．
ymuint
MultiSetPermGen::k() const
{
  return mK;
}

// @brief 先頭の反復子を返す．
MultiSetPermGen::iterator
MultiSetPermGen::begin()
{
  return iterator(this);
}

END_NAMESPACE_YM
