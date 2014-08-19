
/// @file MultiGenBase.cc
/// @brief MultiGenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiGenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiGenBase::MultiGenBase(const vector<pair<ymuint, ymuint> >& nk_array) :
  mNkArray(nk_array),
  mElemArray(nk_array.size())
{
  ymuint ng = mNkArray.size();
  for (ymuint g = 0; g < ng; ++ g) {
    mElemArray[g].resize(k(g));
  }
  init();
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MultiGenBase::MultiGenBase(const MultiGenBase& src) :
  mNkArray(src.mNkArray),
  mElemArray(src.mElemArray)
{
}

// デストラクタ
MultiGenBase::~MultiGenBase()
{
}

// @brief 初期化
void
MultiGenBase::init()
{
  for (ymuint group = 0; group < mNkArray.size(); ++ group) {
    init_group(group);
  }
}

// コピーする．
void
MultiGenBase::copy(const MultiGenBase& src)
{
  if ( this != &src ) {
    mNkArray = src.mNkArray;
    mElemArray = src.mElemArray;
  }
}

// @brief 要素配列の初期化
// @param[in] grp グループ番号
// @note grp 番目のグループの要素配列を初期化する．
void
MultiGenBase::init_group(ymuint grp)
{
  for (ymuint i = 0; i < k(grp); ++ i) {
    elem(grp)[i] = i;
  }
}

END_NAMESPACE_YM
