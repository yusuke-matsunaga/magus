
/// @file SmtVarImpl.cc
/// @brief SmtVarImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtVarImpl.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtVarImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sort 変数の型
SmtVarImpl::SmtVarImpl(const SmtSort* sort) :
  mSort(sort)
{
}

// @brief デストラクタ
SmtVarImpl::~SmtVarImpl()
{
}

// @brief 型を返す．
const SmtSort*
SmtVarImpl::sort() const
{
  return mSort;
}

// @brief global 変数の場合に true を返す．
bool
SmtVarImpl::is_global() const
{
  return false;
}

// @brief forall 変数の場合に true を返す．
bool
SmtVarImpl::is_forall() const
{
  return false;
}

// @brief exists 変数の場合に true を返す．
bool
SmtVarImpl::is_exists() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtGlobalVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sort 変数の型
SmtGlobalVar::SmtGlobalVar(const SmtSort* sort) :
  SmtVarImpl(sort)
{
}

// @brief デストラクタ
SmtGlobalVar::~SmtGlobalVar()
{
}

// @brief global 変数の場合に true を返す．
bool
SmtGlobalVar::is_global() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtForallVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sort 変数の型
SmtForallVar::SmtForallVar(const SmtSort* sort) :
  SmtVarImpl(sort)
{
}

// @brief デストラクタ
SmtForallVar::~SmtForallVar()
{
}

// @brief forall 変数の場合に true を返す．
bool
SmtForallVar::is_forall() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtExistsVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sort 変数の型
SmtExistsVar::SmtExistsVar(const SmtSort* sort) :
  SmtVarImpl(sort)
{
}

// @brief デストラクタ
SmtExistsVar::~SmtExistsVar()
{
}

// @brief exists 変数の場合に true を返す．
bool
SmtExistsVar::is_exists() const
{
  return true;
}

END_NAMESPACE_YM_SMT
