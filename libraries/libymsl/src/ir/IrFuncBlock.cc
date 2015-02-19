
/// @file IrFuncBlock.cc
/// @brief IrFuncBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncBlock.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] toplevel トップレベルブロック
// @param[in] name 関数名
// @param[in] type 関数の型
// @param[in] arg_list 引数のリスト
// @param[in] arg_init_list 引数のデフォルト値のリスト
IrFuncBlock::IrFuncBlock(IrToplevel& toplevel,
			 ShString name,
			 const Type* type,
			 const vector<const IrVar*>& arg_list,
			 const vector<IrNode*>& arg_init_list) :
  mToplevel(toplevel),
  mName(name),
  mType(type),
  mArgList(arg_list),
  mArgInitList(arg_init_list),
  mIndex(0)
{
}

// @brief デストラクタ
IrFuncBlock::~IrFuncBlock()
{
}

// @brief トップレベルのブロックを返す．
IrToplevel&
IrFuncBlock::toplevel()
{
  return mToplevel;
}

// @brief 関数名を返す．
ShString
IrFuncBlock::name() const
{
  return mName;
}

// @brief 関数の型を返す．
const Type*
IrFuncBlock::type() const
{
  return mType;
}

// @brief インデックスを返す．
ymuint
IrFuncBlock::index() const
{
  return mIndex;
}

// @brief インデックスを設定する．
void
IrFuncBlock::set_index(ymuint index)
{
  mIndex = index;
}

END_NAMESPACE_YM_YMSL
