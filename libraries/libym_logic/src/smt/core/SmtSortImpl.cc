
/// @file SmtSortImpl.cc
/// @brief SmtSortImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSortImpl.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtSortImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtSortImpl::SmtSortImpl() :
  mId(kSmtSort_None)
{
}

// @brief デストラクタ
SmtSortImpl::~SmtSortImpl()
{
}

// @brief ID 番号を返す．
// @note ID 番号はすべての型の中で唯一のもの
tSmtSortId
SmtSortImpl::id() const
{
  return mId;
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
tSmtSortId
SmtSortImpl::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return kSmtSort_None;
}

// @brief ハッシュ値を返す．
ymuint32
SmtSortImpl::hash() const
{
  return mId;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtSimpleSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtSimpleSort::SmtSimpleSort()
{
}

// @brief デストラクタ
SmtSimpleSort::~SmtSimpleSort()
{
}


//////////////////////////////////////////////////////////////////////
// クラス SmtComplexSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_list 要素の型のリスト
SmtComplexSort::SmtComplexSort(const vector<tSmtSortId>& elem_list) :
  mElemNum(elem_list.size())
{
  for (ymuint i = 0; i < mElemNum; ++ i) {
    mElemList[i] = elem_list[i];
  }
}

// @brief デストラクタ
SmtComplexSort::~SmtComplexSort()
{
}

// @brief 複合型の場合の要素数を返す．
// @note 単純な型の場合には 0 を返す．
ymuint
SmtComplexSort::elem_num() const
{
  return mElemNum;
}

// @brief 複合型の場合の要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
tSmtSortId
SmtComplexSort::elem(ymuint pos) const
{
  assert_cond( pos < elem_num(), __FILE__, __LINE__);
  return mElemList[pos];
}

END_NAMESPACE_YM_SMT
