
/// @file SmtSort.cc
/// @brief SmtSort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtSort.h"
#include "SmtSortImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtSortImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 型名
SmtSortImpl::SmtSortImpl(const SmtId* name) :
  mName(name)
{
  mLink = NULL;
}

// @brief デストラクタ
SmtSortImpl::~SmtSortImpl()
{
}

// @brief ID番号を返す．
ymuint
SmtSortImpl::id() const
{
  return mId;
}

// @brief 名前を返す．
const SmtId*
SmtSortImpl::name() const
{
  return mName;
}

// @brief 複合型の場合の要素数を返す．
// @note 単純な型の場合には 0 を返す．
ymuint
SmtSortImpl::elem_num() const
{
  return 0;
}

// @brief 複合型の場合の要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
const SmtSort*
SmtSortImpl::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 実際の型を返す．
// @note 通常は自分自身を返すが alias の場合は実体を返す．
const SmtSort*
SmtSortImpl::sort() const
{
  return this;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtAliasSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] sort alias 先の型
SmtAliasSort::SmtAliasSort(const SmtId* name,
			   const SmtSort* sort) :
  SmtSortImpl(name),
  mSort(sort)
{
}

// @brief デストラクタ
SmtAliasSort::~SmtAliasSort()
{
}

// @brief 実際の型を返す．
// @note 通常は自分自身を返すが alias の場合は実体を返す．
const SmtSort*
SmtAliasSort::sort() const
{
  return mSort;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtCplxSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
SmtCplxSort::SmtCplxSort(const SmtId* name) :
  SmtSortImpl(name)
{
  // mElemList は SmtSortMgr が確保する．
}

// @brief デストラクタ
SmtCplxSort::~SmtCplxSort()
{
  // mElemList は SmtSortMgr が確保する．
}

// @brief 複合型の場合の要素数を返す．
// @note 単純な型の場合には 0 を返す．
ymuint
SmtCplxSort::elem_num() const
{
  return mElemNum;
}

// @brief 複合型の場合の要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
const SmtSort*
SmtCplxSort::elem(ymuint pos) const
{
  assert_cond( pos < elem_num(), __FILE__, __LINE__);
  return mElemList[pos];
}

END_NAMESPACE_YM_SMTLIBV2
