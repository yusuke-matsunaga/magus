
/// @file GenBase.cc
/// @brief GenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GenBase.h"


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

END_NAMESPACE_YM
