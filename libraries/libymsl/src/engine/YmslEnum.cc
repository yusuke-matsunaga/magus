
/// @file YmslEnum.cc
/// @brief YmslEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslEnum.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] elem_list 要素のリスト
YmslEnum::YmslEnum(ShString name,
		   const vector<ShString>& elem_list) :
  mName(name),
  mElemNum(elem_list.size())
{
  mElemName = new ShString[mElemNum];
  for (ymuint i = 0; i < mElemNum; ++ i) {
    mElemName[i] = elem_list[i];
  }
}

// @brief デストラクタ
YmslEnum::~YmslEnum()
{
  delete [] mElemName;
}

// @brief 名前を返す．
ShString
YmslEnum::name() const
{
  return mName;
}

// @brief 要素数を返す．
ymuint
YmslEnum::elem_num() const
{
  return mElemNum;
}

// @brief 要素名を返す．
// @param[in] pos 位置 ( 0 <= pos < elem_num() )
ShString
YmslEnum::elem(ymuint pos) const
{
  ASSERT_COND( pos < elem_num() );
  return mElemName[pos];
}

// @brief 名前からインデックスを返す．
// @param[in] elem_name 要素名
//
// 見つからなかった時は -1 を返す．
int
YmslEnum::elem_index(ShString elem_name) const
{
  // とりあえず線形探索
  for (ymuint i = 0; i < mElemNum; ++ i) {
    if ( mElemName[i] == elem_name ) {
      return i;
    }
  }
  return -1;
}

END_NAMESPACE_YM_YMSL
