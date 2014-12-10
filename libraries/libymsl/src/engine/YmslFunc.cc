
/// @file YmslFunc.cc
/// @brief YmslFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFunc.h"
#include "YmslBuiltinFunc.h"
#include "YmslUserFunc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
YmslFunc::YmslFunc(ShString name,
		   const vector<YmslVar*>& arg_list) :
  mName(name)
{
  mArgNum = arg_list.size();
  mArgList = new YmslVar*[mArgNum];
  for (ymuint i = 0; i < mArgNum; ++ i) {
    mArgList[i] = arg_list[i];
  }
}

// @brief デストラクタ
YmslFunc::~YmslFunc()
{
  delete [] mArgList;
}

// @brief 関数名を返す．
ShString
YmslFunc::name() const
{
  return mName;
}

// @brief 引数の数
ymuint
YmslFunc::arg_num() const
{
  return mArgNum;
}

// @brief 引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arg_num() )
YmslVar*
YmslFunc::arg(ymuint pos) const
{
  ASSERT_COND( pos < arg_num() );
  return mArgList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス YmslBuiltinFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
YmslBuiltinFunc::YmslBuiltinFunc(ShString name,
				 const vector<YmslVar*>& arg_list) :
  YmslFunc(name, arg_list)
{
}

// @brief デストラクタ
YmslBuiltinFunc::~YmslBuiltinFunc()
{
}

// @brief 組み込み関数の時 true を返す．
bool
YmslBuiltinFunc::is_builtin() const
{
  return true;
}

// @brief YMSL 関数の場合の局所変数の数を返す．
ymuint
YmslBuiltinFunc::local_var_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief YMSL 関数の場合の局所変数を返す．
// @param[in] lindex 変数インデックス
YmslVar*
YmslBuiltinFunc::local_var(ymuint lindex) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief YMSL 関数の場合のバイトコードのサイズを返す．
ymuint
YmslBuiltinFunc::byte_codes_size() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief YMSL 関数の場合のバイトコードの先頭アドレスを返す．
const ymuint8*
YmslBuiltinFunc::byte_codes_top() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslUserFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @param[in] local_var_list ローカル変数のリスト
// @param[in] byte_codes バイトコード
YmslUserFunc::YmslUserFunc(ShString name,
			   const vector<YmslVar*>& arg_list,
			   const vector<YmslVar*>& local_var_list,
			   const vector<ymuint8>& byte_codes) :
  YmslFunc(name, arg_list)
{
  mLocalVarNum = local_var_list.size();
  mLocalVarTable = new YmslVar*[mLocalVarNum];
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
YmslUserFunc::~YmslUserFunc()
{
  delete [] mLocalVarTable;
  delete [] mByteCodes;
}

// @brief 組み込み関数の時 true を返す．
bool
YmslUserFunc::is_builtin() const
{
  return false;
}

// @brief 組み込み関数の時の実行関数
void
YmslUserFunc::execute() const
{
  ASSERT_NOT_REACHED;
}

// @brief YMSL 関数の場合の局所変数の数を返す．
ymuint
YmslUserFunc::local_var_num() const
{
  return mLocalVarNum;
}

// @brief YMSL 関数の場合の局所変数を返す．
// @param[in] lindex 変数インデックス
YmslVar*
YmslUserFunc::local_var(ymuint lindex) const
{
  ASSERT_COND( lindex < local_var_num() );
  return mLocalVarTable[lindex];
}

// @brief YMSL 関数の場合のバイトコードのサイズを返す．
ymuint
YmslUserFunc::byte_codes_size() const
{
  return mByteCodesSize;
}

// @brief YMSL 関数の場合のバイトコードの先頭アドレスを返す．
const ymuint8*
YmslUserFunc::byte_codes_top() const
{
  return mByteCodes;
}

END_NAMESPACE_YM_YMSL
