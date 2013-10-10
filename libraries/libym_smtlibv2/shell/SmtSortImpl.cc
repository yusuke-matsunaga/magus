
/// @file SmtSortImpl.cc
/// @brief SmtSortImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


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

// @brief 名前を返す．
const SmtId*
SmtSortImpl::name() const
{
  return mName;
}

// @brief パラメータ番号を返す．
// @note 通常の型の場合は -1 を返す．
ymint
SmtSortImpl::param_id() const
{
  return -1;
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

// @brief 型テンプレートを返す．
const SmtSort*
SmtSortImpl::sort_template() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief ハッシュ値を返す．
ymuint
SmtSortImpl::hash() const
{
  return mId + mLevel * 2047;
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
  mSortTemplate(sort)
{
}

// @brief デストラクタ
SmtAliasSort::~SmtAliasSort()
{
}

// @brief 型テンプレートを返す．
const SmtSort*
SmtAliasSort::sort_template() const
{
  return mSortTemplate;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtCplxSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] elem_list 要素のリスト
SmtCplxSort::SmtCplxSort(const SmtId* name,
			 const vector<const SmtSort*>& elem_list) :
  SmtSortImpl(name),
  mElemNum(elem_list.size())
{
  // mElemList は SmtSortMgr が確保する．
  for (ymuint i = 0; i < mElemNum; ++ i) {
    mElemList[i] = elem_list[i];
  }
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


//////////////////////////////////////////////////////////////////////
// クラス SmtParamSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pid パラメータ番号
SmtParamSort::SmtParamSort(ymuint pid) :
  mParamId(pid)
{
}

// @brief デストラクタ
SmtParamSort::~SmtParamSort()
{
}

// @brief 名前を返す．
const SmtId*
SmtParamSort::name() const
{
  return NULL;
}

// @brief パラメータ番号を返す．
// @note 通常の型の場合は -1 を返す．
ymint
SmtParamSort::param_id() const
{
  return mParamId;
}

// @brief 複合型の場合の要素数を返す．
// @note 単純な型の場合には 0 を返す．
ymuint
SmtParamSort::elem_num() const
{
  return 0;
}

// @brief 複合型の場合の要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
const SmtSort*
SmtParamSort::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 型テンプレートを返す．
const SmtSort*
SmtParamSort::sort_template() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief ハッシュ値を返す．
ymuint
SmtParamSort::hash() const
{
  return 0;
}

END_NAMESPACE_YM_SMTLIBV2
