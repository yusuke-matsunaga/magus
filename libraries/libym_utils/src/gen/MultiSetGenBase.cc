
/// @file MultiSetGenBase.cc
/// @brief MultiSetGenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiSetGenBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
MultiSetGenBase::MultiSetGenBase(const vector<ymuint>& num_array,
				 ymuint k) :
  mNumArray(num_array),
  mK(k),
  mElem(k)
{
  init();
}

// @brief デストラクタ
MultiSetGenBase::~MultiSetGenBase()
{
}

// @brief 初期化する．
void
MultiSetGenBase::init()
{
  ymuint pos = 0;
  ymuint count = 0;
  for (ymuint i = 0; i < mK; ++ i) {
    if ( count >= mNumArray[pos] ) {
      ++ pos;
      count = 0;
    }
    assert_cond( count < mNumArray[pos], __FILE__, __LINE__);
    mElem[i] = pos;
    ++ count;
  }
}

// @brief 内容をコピーする関数
// @param[in] src コピー元のオブジェクト
void
MultiSetGenBase::copy(const MultiSetGenBase& src)
{
  mNumArray = src.mNumArray;
  mK = src.mK;
  mElem = src.mElem;
}

END_NAMESPACE_YM
