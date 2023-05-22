
/// @file Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Cut.h"


BEGIN_NAMESPACE_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス Cut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] nl 葉の数(入力数)
Cut::Cut(SizeType nl) :
  mLeafNum(nl)
{
  if ( nl > 0 ) {
    mLeafArray = new PtrIntType[nl];
  }
  else {
    mLeafArray = nullptr;
  }
}

// @brief コピーコンストラクタ
Cut::Cut(const Cut& src) :
  mLeafNum(src.leaf_num())
{
  if ( mLeafNum > 0 ) {
    mLeafArray = new PtrIntType[mLeafNum];
    for ( SizeType i = 0; i < mLeafNum; ++ i) {
      mLeafArray[i] = src.mLeafArray[i];
    }
  }
  else {
    mLeafArray = nullptr;
  }
}

// @brief 代入演算子
const Cut&
Cut::operator=(const Cut& src)
{
  resize(src.leaf_num());
  for ( SizeType i = 0; i < mLeafNum; ++ i) {
    mLeafArray[i] = src.mLeafArray[i];
  }

  return *this;
}

// @brief デストラクタ
Cut::~Cut()
{
  delete [] mLeafArray;
}

END_NAMESPACE_CELLMAP
