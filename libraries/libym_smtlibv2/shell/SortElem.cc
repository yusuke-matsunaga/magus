
/// @file SortElem.cc
/// @brief SortElem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SortElem.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtTemplBase
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
SortElem::~SortElem()
{
}

// @brief パラメータ型の場合に true を返す．
bool
SortElem::is_param() const
{
  return false;
}

// @brief パラメータ番号を返す．
// @note is_param() == true の時のみ意味を持つ．
ymuint
SortElem::param_id() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 名前を返す．
// @note is_param() == false の時のみ意味を持つ．
const SmtId*
SortElem::name() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 要素数を返す．
// @note is_param() == false の時のみ意味を持つ．
ymuint
SortElem::elem_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
// @note is_param() == false の時のみ意味を持つ．
const SortElem*
SortElem::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス ParamSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] param_id パラメータ番号
ParamSort::ParamSort(ymuint param_id) :
  mParamId(param_id)
{
}

// @brief デストラクタ
ParamSort::~ParamSort()
{
}

// @brief パラメータ型の場合に true を返す．
bool
ParamSort::is_param() const
{
  return true;
}

// @brief パラメータ番号を返す．
// @note is_param() == true の時のみ意味を持つ．
ymuint
ParamSort::param_id() const
{
  return mParamId;
}


//////////////////////////////////////////////////////////////////////
// クラス SimplSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_id 名前を表す識別子
SimpleSort::SimpleSort(const SmtId* name_id) :
  mName(name_id)
{
}

// @brief デストラクタ
SimpleSort::~SimpleSort()
{
}

// @brief 名前を返す．
// @note is_param() == false の時のみ意味を持つ．
const SmtId*
SimpleSort::name() const
{
  return mName;
}

// @brief 要素数を返す．
// @note is_param() == false の時のみ意味を持つ．
ymuint
SimpleSort::elem_num() const
{
  return 0;
}

// @brief 要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
// @note is_param() == false の時のみ意味を持つ．
const SortElem*
SimpleSort::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス ComplexSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_id 名前を表す識別子
// @param[in] elem_list 要素の型のリスト
ComplexSort::ComplexSort(const SmtId* name_id,
			 const vector<const SortElem*>& elem_list) :
  SimpleSort(name_id),
  mElemNum(elem_list.size())
{
  for (ymuint i = 0; i < mElemNum; ++ i) {
    mElemList[i] = elem_list[i];
  }
}

// @brief デストラクタ
ComplexSort::~ComplexSort()
{
}

// @brief 要素数を返す．
// @note is_param() == false の時のみ意味を持つ．
ymuint
ComplexSort::elem_num() const
{
  return mElemNum;
}

// @brief 要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
// @note is_param() == false の時のみ意味を持つ．
const SortElem*
ComplexSort::elem(ymuint pos) const
{
  assert_cond( pos < elem_num(), __FILE__, __LINE__);
  return mElemList[pos];
}

END_NAMESPACE_YM_SMTLIBV2
