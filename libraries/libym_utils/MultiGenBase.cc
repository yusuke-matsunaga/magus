
/// @file MultiGenBase.cc
/// @brief MultiGenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiGenIterator
//////////////////////////////////////////////////////////////////////

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

END_NAMESPACE_YM
