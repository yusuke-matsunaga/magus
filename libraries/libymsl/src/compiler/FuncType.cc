
/// @file FuncType.cc
/// @brief FuncType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FuncType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス FuncType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
FuncType::FuncType(const Type* output_type,
		   const vector<const Type*>& input_type_list) :
  mOutputType(output_type),
  mInputType(input_type_list.size())
{
  for (ymuint i = 0; i < mInputType.size(); ++ i) {
    mInputType[i] = input_type_list[i];
  }
}

// @brief デストラクタ
FuncType::~FuncType()
{
}

// @brief 型を得る．
TypeId
FuncType::type_id() const
{
  return kFuncType;
}

// @brief 関数の出力の型を返す．
//
// function のみ有効
const Type*
FuncType::function_output_type() const
{
  return mOutputType;
}

// @brief 関数の入力数を返す．
//
// function のみ有効
ymuint
FuncType::function_input_num() const
{
  return mInputType.size();
}

// @brief 関数の入力の型を返す．
// @param[in] pos 入力番号 ( 0 <= pos < function_input_num() )
//
// function のみ有効
const Type*
FuncType::function_input_type(ymuint pos) const
{
  ASSERT_COND( pos < function_input_num() );
  return mInputType[pos];
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
FuncType::print(ostream& s) const
{
  s << "function(";
  ymuint n = function_input_num();
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    s << comma;
    function_input_type(i)->print(s);
    comma = ", ";
  }
  s << ") : ";
  function_output_type()->print(s);
}

END_NAMESPACE_YM_YMSL
