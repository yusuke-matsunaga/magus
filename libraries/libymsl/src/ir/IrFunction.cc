
/// @file IrFunction.cc
/// @brief IrFunction の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
// @param[in] index インデックス
IrFunction::IrFunction(ShString name,
			   const IrType* output_type,
			   const vector<const IrType*>& input_type_list,
			   int index) :
  mName(name),
  mOutputType(output_type),
  mInputNum(input_type_list.size()),
  mIndex(index)
{
  mInputType = new const IrType*[mInputNum];
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mInputType[i] = input_type_list[i];
  }
}

// @brief デストラクタ
IrFunction::~IrFunction()
{
  delete [] mInputType;
}

// @brief 関数名を返す．
ShString
IrFunction::name() const
{
  return mName;
}

// @brief 出力の型を返す．
const IrType*
IrFunction::output_type() const
{
  return mOutputType;
}

// @brief 入力数を返す．
ymuint
IrFunction::input_num() const
{
  return mInputNum;
}

// @brief 入力の型の返す．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
const IrType*
IrFunction::input_type(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputType[pos];
}

// @brief 関数インデックスを返す．
int
IrFunction::index() const
{
  return mIndex;
}


#if 0
//////////////////////////////////////////////////////////////////////
// クラス IrBuiltinFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
IrBuiltinFunc::IrBuiltinFunc(ShString name,
				 const vector<IrVar*>& arg_list) :
  IrFunc(name, arg_list)
{
}

// @brief デストラクタ
IrBuiltinFunc::~IrBuiltinFunc()
{
}

// @brief 組み込み関数の時 true を返す．
bool
IrBuiltinFunc::is_builtin() const
{
  return true;
}

// @brief YMSL 関数の場合の局所変数の数を返す．
ymuint
IrBuiltinFunc::local_var_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief YMSL 関数の場合の局所変数を返す．
// @param[in] lindex 変数インデックス
IrVar*
IrBuiltinFunc::local_var(ymuint lindex) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief YMSL 関数の場合のバイトコードのサイズを返す．
ymuint
IrBuiltinFunc::byte_codes_size() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief YMSL 関数の場合のバイトコードの先頭アドレスを返す．
const ymuint8*
IrBuiltinFunc::byte_codes_top() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス IrUserFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @param[in] local_var_list ローカル変数のリスト
// @param[in] byte_codes バイトコード
IrUserFunc::IrUserFunc(ShString name,
			   const vector<IrVar*>& arg_list,
			   const vector<IrVar*>& local_var_list,
			   const vector<ymuint8>& byte_codes) :
  IrFunc(name, arg_list)
{
  mLocalVarNum = local_var_list.size();
  mLocalVarTable = new IrVar*[mLocalVarNum];
  for (ymuint i = 0; i < mLocalVarNum; ++ i) {
    mLocalVarTable[i] = local_var_list[i];
  }

  mByteCodesSize = byte_codes.size();
  mByteCodes = new ymuint8[mByteCodesSize];
  for (ymuint i = 0; i < mByteCodesSize; ++ i) {
    mByteCodes[i] = byte_codes[i];
  }
}

// @brief デストラクタ
IrUserFunc::~IrUserFunc()
{
  delete [] mLocalVarTable;
  delete [] mByteCodes;
}

// @brief 組み込み関数の時 true を返す．
bool
IrUserFunc::is_builtin() const
{
  return false;
}

// @brief 組み込み関数の時の実行関数
void
IrUserFunc::execute() const
{
  ASSERT_NOT_REACHED;
}

// @brief YMSL 関数の場合の局所変数の数を返す．
ymuint
IrUserFunc::local_var_num() const
{
  return mLocalVarNum;
}

// @brief YMSL 関数の場合の局所変数を返す．
// @param[in] lindex 変数インデックス
IrVar*
IrUserFunc::local_var(ymuint lindex) const
{
  ASSERT_COND( lindex < local_var_num() );
  return mLocalVarTable[lindex];
}

// @brief YMSL 関数の場合のバイトコードのサイズを返す．
ymuint
IrUserFunc::byte_codes_size() const
{
  return mByteCodesSize;
}

// @brief YMSL 関数の場合のバイトコードの先頭アドレスを返す．
const ymuint8*
IrUserFunc::byte_codes_top() const
{
  return mByteCodes;
}
#endif

END_NAMESPACE_YM_YMSL
