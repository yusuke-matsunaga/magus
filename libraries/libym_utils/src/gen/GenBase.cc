
/// @file GenBase.cc
/// @brief GenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
GenBase::GenBase(ymuint n,
		 ymuint k) :
  mN(n),
  mK(k),
  mElem(k)
{
  init();
}

// @brief コピーコンストラクタ
GenBase::GenBase(const GenBase& src) :
  mN(src.mN),
  mK(src.mK),
  mElem(src.mElem)
{
}

// デストラクタ
GenBase::~GenBase()
{
}

// @brief 最初の要素を指すように初期化する．
void
GenBase::init()
{
  for (ymuint i = 0; i < mK; ++ i) {
    mElem[i] = i;
  }
}

// 内容をコピーする関数
void
GenBase::copy(const GenBase& src)
{
  mN = src.mN;
  mK = src.mK;
  if ( mElem.size() != mK ) {
    mElem.resize(mK);
  }
  for (ymuint i = 0; i < mK; ++ i) {
    mElem[i] = src.mElem[i];
  }
}

END_NAMESPACE_YM
