
/// @file SmtSortImpl.cc
/// @brief SmtSortImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSortImpl.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtSortImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
SmtSortImpl::SmtSortImpl(ymuint id) :
  mId(id)
{
}

// @brief デストラクタ
SmtSortImpl::~SmtSortImpl()
{
}

// @brief 組み込み型を返す．
// @note 普通の型は kSmtSort_UserDef を返す．
tSmtSort
SmtSortImpl::type() const
{
  return kSmtSort_UserDef;
}

// @brief ID 番号を返す．
// @note ID 番号はすべての型の中で唯一のもの
ymuint
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
const SmtSort*
SmtSortImpl::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief ハッシュ値を返す．
ymuint
SmtSortImpl::hash() const
{
  return mId;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtBoolSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
SmtBoolSort::SmtBoolSort(ymuint id) :
  SmtSortImpl(id)
{
}

// @brief デストラクタ
SmtBoolSort::~SmtBoolSort()
{
}

// @brief 組み込み型を返す．
// @note 普通の型は kSmtSort_UserDef を返す．
tSmtSort
SmtBoolSort::type() const
{
  return kSmtSort_Bool;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtIntSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
SmtIntSort::SmtIntSort(ymuint id) :
  SmtSortImpl(id)
{
}

// @brief デストラクタ
SmtIntSort::~SmtIntSort()
{
}

// @brief 組み込み型を返す．
// @note 普通の型は kSmtSort_UserDef を返す．
tSmtSort
SmtIntSort::type() const
{
  return kSmtSort_Int;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtRealSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
SmtRealSort::SmtRealSort(ymuint id) :
  SmtSortImpl(id)
{
}

// @brief デストラクタ
SmtRealSort::~SmtRealSort()
{
}

// @brief 組み込み型を返す．
// @note 普通の型は kSmtSort_UserDef を返す．
tSmtSort
SmtRealSort::type() const
{
  return kSmtSort_Real;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtSimpleSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
SmtSimpleSort::SmtSimpleSort(ymuint id) :
  SmtSortImpl(id)
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
// @param[in] id ID番号
// @param[in] elem_list 要素の型のリスト
SmtComplexSort::SmtComplexSort(ymuint id,
			       const vector<const SmtSort*>& elem_list) :
  SmtSortImpl(id),
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
const SmtSort*
SmtComplexSort::elem(ymuint pos) const
{
  assert_cond( pos < elem_num(), __FILE__, __LINE__);
  return mElemList[pos];
}

END_NAMESPACE_YM_SMT
