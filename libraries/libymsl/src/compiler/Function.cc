
/// @file Function.cc
/// @brief Function の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Function.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Function
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
// @param[in] index インデックス
Function::Function(ShString name,
		   const Type* output_type,
		   const vector<const Type*>& input_type_list,
		   int index) :
  mName(name),
  mOutputType(output_type),
  mInputNum(input_type_list.size()),
  mIndex(index)
{
  mInputType = new const Type*[mInputNum];
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mInputType[i] = input_type_list[i];
  }
}

// @brief デストラクタ
Function::~Function()
{
  delete [] mInputType;
}

// @brief 関数名を返す．
ShString
Function::name() const
{
  return mName;
}

// @brief 出力の型を返す．
const Type*
Function::output_type() const
{
  return mOutputType;
}

// @brief 入力数を返す．
ymuint
Function::input_num() const
{
  return mInputNum;
}

// @brief 入力の型の返す．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
const Type*
Function::input_type(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputType[pos];
}

// @brief 関数インデックスを返す．
int
Function::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
