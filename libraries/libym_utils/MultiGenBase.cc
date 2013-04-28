
/// @file MultiGenBase.cc
/// @brief MultiGenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiGenBase
//////////////////////////////////////////////////////////////////////

// コピーする．
void
MultiGenBase::copy(const MultiGenBase& src)
{
  if ( this != &src ) {
    mNKArray = src.mNKArray;
    mElemArray = src.mElemArray;
  }
}

// grp 番目のグループの要素配列を初期化する．
void
MultiGenIterator::init(ymuint grp)
{
  for (ymuint i = 0; i < k(grp); ++ i) {
    elem(grp)[i] = i;
  }
}

END_NAMESPACE_YM
