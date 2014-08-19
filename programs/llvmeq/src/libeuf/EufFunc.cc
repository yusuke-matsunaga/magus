
/// @file EufFunc.cc
/// @brief EufFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufFunc.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
EufFunc::EufFunc(ymuint id,
		 const string& name,
		 const vector<EufNode*>& arg_list) :
  EufNode(id, VarId(0)),
  mName(name),
  mArgList(arg_list)
{
}

/// @brief デストラクタ
EufFunc::~EufFunc()
{
}

// @brief 型を得る．
EufNode::tType
EufFunc::type() const
{
  return kFunc;
}

// @brief Boolean 型の時 true を返す．
bool
EufFunc::is_boolean() const
{
  return false;
}

// @brief 識別子名を得る．
// @note type() が kFunc, kVar の時のみ有効
string
EufFunc::id_name() const
{
  return mName;
}

// @brief 引数の数を得る．
// @note type() が kFunc の時のみ有効
ymuint
EufFunc::arg_num() const
{
  return mArgList.size();
}

// @brief 引数を得る．
// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
// @note type() が kFunc の時のみ有効
EufNode*
EufFunc::arg(ymuint pos) const
{
  assert_cond( pos < mArgList.size(), __FILE__, __LINE__);
  return mArgList[pos];
}

END_NAMESPACE_YM_LLVMEQ
