
/// @file SmtSort.cc
/// @brief SmtSort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtSort.h"
#include "SmtCplxSort.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 型名
SmtSort::SmtSort(const SmtId* name) :
  mName(name)
{
}

// @brief デストラクタ
SmtSort::~SmtSort()
{
}

// @brief 複合型の場合の要素数を返す．
// @note 単純な型の場合には 0 を返す．
ymuint
SmtSort::elem_num() const
{
  return 0;
}

// @brief 複合型の場合の要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
const SmtSort*
SmtSort::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtCplxSort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
SmtCplxSort::SmtCplxSort(const SmtId* name) :
  SmtSort(name)
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
